#pragma once

//Internal macro
#define _GET_VARG_REMAP(t, vararg) \
template<>\
[[gnu::always_inline]]\
inline t getVArg<t>(va_list args) {\
 return (t)va_arg(args, vararg);\
}

//Internal macro
#define _GET_AARG_MAP(t, member) \
template<>\
class JValueArgResolver<false, t> {\
public:\
 inline static constexpr const bool isRegisteredResolver = true;\
 [[gnu::always_inline]]\
 inline static t getAArg(jvalue *values) {\
  return (t)values->member;\
 }\
};

//Internal macro
#define _ASSERT_JNI_FUNCTION_COMPLIANCE \
static_assert(\
 _CX::VerifyJNIArguments<Args...>::verify(),\
 "Registered JNI functions may only accept JNI types and pointers to _jobject or derived classes!"\
);

//Checks a given class for a inlined static function called 'cast'
DEFINE_FUNCTION_DETECTOR(cast)

namespace FakeJni {
 //C to C++ Vararg glue
 namespace _CX {
  template<bool, typename>
  class JValueArgResolver {
  public:
   inline static constexpr const bool isRegisteredResolver = false;
  };

  //JValueArgResolver for JObject derived classes
  template<typename T>
  class JValueArgResolver<true, T> {
  private:
   template<typename R, typename TT, typename... Args>
   inline static constexpr bool assertCastMatches(R (TT::*)(Args...)) {
    return false;
   }

   template<typename R, typename... Args>
   inline static constexpr bool assertCastMatches(R (*)(Args...)) {
    if constexpr(std::is_same<componentType *, R>::value && sizeof...(Args) == 1) {
     using arg_t = typename CX::TemplateTypeIterator<0, Args...>::type;
     return std::is_same<_jobject *, arg_t>::value;
    }
    return false;
   }

  public:
   inline static constexpr const bool isRegisteredResolver = true;
   using componentType = typename ComponentTypeResolver<T*>::type;

   [[gnu::always_inline]]
   inline static componentType* getAArg(jvalue *values) {
    static_assert(std::is_base_of<_jobject, componentType>::value, "Illegal JNI function parameter type!");
    if constexpr(CX::HascastFunction<componentType>::value) {
     static_assert(
      assertCastMatches(&componentType::cast),
      "Illegal prototype for downcasting delegate! The prototype should be:"
      "\n\tinline static Derived* cast(_jobject *) {}"
      "\n(where 'Derived' is your native class)"
     );
     return componentType::cast(values->l);
    } else {
     return (componentType *)values->l;
    }
   }
  };

  //Function breeder for JValueArgResolver<typename>::getAArg(jvalue *)
  template<typename T>
  [[gnu::always_inline]]
  inline static T getAArg(jvalue *values) {
   using componentType = typename ComponentTypeResolver<T>::type;
   static_assert(
    JValueArgResolver<false, componentType>::isRegisteredResolver || JValueArgResolver<true, componentType>::isRegisteredResolver,
    "Illegal JNI function parameter type!"
   );
   return JValueArgResolver<std::is_class<componentType>::value, T>::getAArg(values);
  }

  //
  template<typename T>
  [[gnu::always_inline]]
  inline static T getVArg(va_list args) {
   return va_arg(args, T);
  }

  //TODO add the rest of the va_arg type promotions
  _GET_VARG_REMAP(char, int)
  _GET_VARG_REMAP(short, int)
  _GET_VARG_REMAP(float, double)

  //Type-to-member-name maps for the jvalue union
  _GET_AARG_MAP(JBoolean, z)
  _GET_AARG_MAP(JByte, b)
  _GET_AARG_MAP(JChar, c)
  _GET_AARG_MAP(JShort, s)
  _GET_AARG_MAP(JInt, i)
  _GET_AARG_MAP(JLong, j)
  _GET_AARG_MAP(JFloat, f)
  _GET_AARG_MAP(JDouble, d)

  template<auto, typename...>
  class FunctionAccessor {};

  //Entry invoker for member functions
  template<auto N, typename T, typename R, typename... Args>
  class FunctionAccessor<N, R (T::* const)(Args...)> {
  public:
   using argType = typename CX::TemplateTypeIterator<N - 1, Args...>::type;
   using functionType = R (T::* const)(Args...);
   using erasedType = void (AnyClass::* const)();

   template<typename... DecomposedVarargs>
   [[gnu::always_inline]]
   inline static R invokeV(void *const inst, erasedType func, va_list list, DecomposedVarargs... args) {
    return FunctionAccessor<N - 1, functionType>::template invokeV<argType, DecomposedVarargs...>(
     inst,
     func,
     list,
     getVArg<argType>(list),
     args...
    );
   }

   template<typename... DecomposedJValues>
   [[gnu::always_inline]]
   inline static R invokeA(void * const inst, erasedType func, jvalue *values, DecomposedJValues... args) {
    return FunctionAccessor<N - 1, functionType>::template invokeA<argType, DecomposedJValues...>(
     inst,
     func,
     values + 1,
     getAArg<argType>(values),
     args...
    );
   }
  };

  //Base invoker for member functions
  template<typename T, typename R, typename... Args>
  class FunctionAccessor<0, R (T::* const)(Args...)> {
  public:
   using erasedType = void (AnyClass::* const)();
   //Template pack should match Args
   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeV(void * const inst, erasedType func, va_list list, Args2... args) {
//    static_assert(
//     std::is_same<std::tuple<Args...>, std::tuple<Args2...>>::value,
//     "Function argument list does not match base invoker arguments!"
//    );
    va_end(list);
//    return (((T*)inst)->*((R (T::*)(Args...))func))(args...);
    return invokeA(inst, func, nullptr, args...);
   }

   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeA(void * const inst, erasedType func, jvalue *values, Args2... args) {
    static_assert(
     std::is_same<std::tuple<Args...>, std::tuple<Args2...>>::value,
     "Function argument list does not match base invoker arguments!"
    );
    return (((T*)inst)->*((R (T::*)(Args...))func))(args...);
   }
  };

  //Entry invoker for non-member functions
  template<auto N, typename R, typename... Args>
  class FunctionAccessor<N, R (* const)(Args...)> {
  public:
   using argType = typename CX::TemplateTypeIterator<N - 1, Args...>::type;
   using functionType = R (* const)(Args...);
   using erasedType = void (* const)();

   template<typename... DecomposedVarargs>
   [[gnu::always_inline]]
   inline static R invokeV(erasedType func, va_list list, DecomposedVarargs... args) {
    return FunctionAccessor<N - 1, functionType>::template invokeV<argType, DecomposedVarargs...>(
     func,
     list,
     getVArg<argType>(list),
     args...
    );
   }

   template<typename... DecomposedJValues>
   [[gnu::always_inline]]
   inline static R invokeA(erasedType func, jvalue *values, DecomposedJValues... args) {
    return FunctionAccessor<N - 1, functionType>::template invokeA<argType, DecomposedJValues...>(
     func,
     values + 1,
     getAArg<argType>(values),
     args...
    );
   }
  };

  //Base invoker for non-member functions
  template<typename R, typename... Args>
  class FunctionAccessor<0, R (* const)(Args...)> {
  public:
   using erasedType = void (* const)();

   //Template pack should match Args
   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeV(erasedType func, va_list list, Args2... args) {
//    static_assert(
//     std::is_same<std::tuple<Args...>, std::tuple<Args2...>>::value,
//     "Function argument list does not match base invoker arguments!"
//    );
    va_end(list);
//    using f_t = R (* const)(Args...);
//    return ((f_t)func)(args...);
    return invokeA(func, nullptr, args...);
   }

   template<typename... Args2>
   inline static R invokeA(erasedType func, jvalue *values, Args2... args) {
    static_assert(
     std::is_same<std::tuple<Args...>, std::tuple<Args2...>>::value,
     "Function argument list does not match base invoker arguments!"
    );
    return ((R (*)(Args...))func)(args...);
   }
  };

  //SFINAE functions to generate JNI function signatures
  //Fold expression evaluators
  //Evaluator for non-empty fold
  template<typename... Args>
  class EvaluateFold {
  public:
   inline static constexpr const auto fold = CX::Concat<(JniTypeBase<Args>::signature, ...)>::result;
  };

  //Evaluator for empty folds
  template<>
  class EvaluateFold<> {
  public:
   inline static constexpr const char fold[] = "";
  };

  template<bool IsConstructor, typename R, typename... Args>
  class SignatureGenerator;

  //Generator for regular functions
  template<typename R, typename... Args>
  class SignatureGenerator<false, R, Args...> {
  public:
   inline static constexpr const char
    prefix[] = "(",
    suffix[] = ");";
   inline static constexpr const auto signature =
    CX::Concat<prefix, EvaluateFold<Args...>::fold, suffix, JniTypeBase<R>::signature>::result;
  };

  //Generator for constructors
  template<typename R, typename... Args>
  class SignatureGenerator<true, R, Args...> {
  public:
   inline static constexpr const char suffix[] = ");V";
   inline static constexpr const auto signature =
    CX::Concat<SignatureGenerator<false, R, Args...>::prefix, EvaluateFold<Args...>::fold, suffix>::result;
  };
 }

 //JNI base
 struct _jmethodID {};

 //fake-jni implementation
 class JMethodID: public _jmethodID, private JNINativeMethod {
 private:
  using staticFunc_t = void (* const)();
  using memberFunc_t = void (_CX::AnyClass::* const)();

  //Describes encapsulated function, not JVM modifier
  const bool isStatic;
  //Describes JVM modifier
  //A modifier of 0 indicates a native method registered through `registerNatives`
  const uint32_t modifiers;
  union {
   staticFunc_t staticFunc;
   memberFunc_t memberFunc;
  };
  void
   (* const proxyFuncV)(),
   (* const proxyFuncA)();

 public:
  //Currently JVM modifiers do nothing
  //TODO ensure that these are correct
  //METHOD_MODIFIERS = 3391;
  enum Modifiers: uint32_t {
   PUBLIC = 1,
   PRIVATE = 2,
   PROTECTED = 4,
   STATIC = 8,
   FINAL = 16,
   SYNCHRONIZED = 32,
   NATIVE = 256,
   ABSTRACT = 1024,
   STRICT = 2048,
   SYNTHETIC = 4096,
   MANDATED = 32768
  };

  //Constructor for delegate constructors
  template<typename R, typename... Args>
  JMethodID(R (* const func)(Args...), const uint32_t modifiers) noexcept :
   _jmethodID(),
   JNINativeMethod {
    const_cast<char *>("<init>"),
    const_cast<char *>(_CX::SignatureGenerator<true, R, Args...>::signature),
    nullptr
   },
   isStatic(true),
   modifiers(modifiers),
   staticFunc((staticFunc_t)func),
   proxyFuncV((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeV<>),
   proxyFuncA((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeA<>)
  {
   _ASSERT_JNI_FUNCTION_COMPLIANCE
  }

  //constructor for static functions
  template<typename R, typename... Args>
  JMethodID(R (* const func)(Args...), const char * const name, const uint32_t modifiers) noexcept :
   _jmethodID(),
   JNINativeMethod {
    const_cast<char *>(name),
    const_cast<char *>(_CX::SignatureGenerator<false, R, Args...>::signature),
    nullptr
   },
   isStatic(true),
   modifiers(modifiers),
   staticFunc((staticFunc_t)func),
   proxyFuncV((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeV<>),
   proxyFuncA((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeA<>)
  {
   _ASSERT_JNI_FUNCTION_COMPLIANCE
  }

  //Constructor for member methods
  template<typename T, typename R, typename... Args>
  JMethodID(R (T::* const func)(Args...), const char * const name, const uint32_t modifiers) noexcept :
   _jmethodID(),
   //JNINativeMethod does not store const pointers to anything
   //const_cast is safe since we inherit privately from JNINativeMethod
   //and there is no risk of overwriting immutable pointers
   JNINativeMethod {
    const_cast<char *>(name),
    const_cast<char *>(_CX::SignatureGenerator<false, R, Args...>::signature),
    nullptr
   },
   isStatic(false),
   modifiers(modifiers),
   memberFunc((memberFunc_t)func),
   proxyFuncV((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeV<>),
   proxyFuncA((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeA<>)
  {
   _ASSERT_JNI_FUNCTION_COMPLIANCE
  }

  bool operator ==(JMethodID &mid) const noexcept {
   return (name == mid.name)
    && (strcmp(signature, mid.signature) == 0)
    && (isStatic == mid.isStatic)
    && (modifiers == mid.modifiers)
    && (isStatic ? (memberFunc == mid.memberFunc) : (staticFunc == mid.staticFunc))
    && (proxyFuncV == mid.proxyFuncV)
    && (proxyFuncA == mid.proxyFuncA);
  }

  const char * getName() const noexcept {
   return name;
  }

  const char * getSignature() const noexcept {
   return signature;
  }

  uint32_t getModifiers() const noexcept {
   return modifiers;
  }

  template<typename R>
  [[gnu::always_inline]]
  inline R invoke(void * const inst, va_list list) const {
   if (!modifiers) {
    throw std::runtime_error("Functions linked through `registerNatives` are not supported yet!");
   } else {
    if (isStatic) {
     return ((R (*)(staticFunc_t, va_list))proxyFuncV)(staticFunc, list);
    } else {
     return ((R (*)(void * const, memberFunc_t, va_list))proxyFuncV)(inst, memberFunc, list);
    }
   }
  }

  template<typename R>
  [[gnu::always_inline]]
  inline R invoke(void * const inst, const jvalue *values) {
   if (!modifiers) {
    throw std::runtime_error("Functions linked through `registerNatives` are not supported yet!");
   } else {
    if (isStatic) {
     return ((R (*)(staticFunc_t, jvalue *))proxyFuncA)(staticFunc, values);
    } else {
     return ((R (*)(void * const, memberFunc_t, jvalue *))proxyFuncA)(inst, memberFunc, values);
    }
   }
  }
 };
}