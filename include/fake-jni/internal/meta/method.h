#pragma once

#include "jni.h"

#include "meta.h"

#include <cx/strings.h>

#include <tuple>

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

//Template glue code for method registration and access
namespace FakeJni {
 namespace _CX {
  //Ensures that all type arguments are valid JNI parameters
  template<typename... Args>
  class VerifyJniFunctionArguments {
  public:
   [[gnu::always_inline]]
   inline static constexpr bool verify() {
    return  !((!VerifyJniFunctionArguments<Args>::verify()) || ...);
   }
  };

  template<typename T>
  class VerifyJniFunctionArguments<T> {
  public:
   [[gnu::always_inline]]
   inline static constexpr bool verify() {
    using resolver = ComponentTypeResolver<T>;
    if constexpr(__is_class(typename resolver::type)) {
     return __is_base_of(_jobject, typename resolver::type) && resolver::indirectionCount == 1U;
    } else {
     return JniTypeBase<T>::isRegisteredType && resolver::indirectionCount == 0U;
    }
   }
  };

  template<>
  class VerifyJniFunctionArguments<> {
  public:
   [[gnu::always_inline]]
   inline static constexpr bool verify() {
    return true;
   }
  };

  //C to C++ Vararg glue
  template<bool IsClass, typename T>
  class JValueArgResolver {
  public:
   inline static constexpr const bool isRegisteredResolver = false;
  };

  //Function breeder for JValueArgResolver<typename>::getAArg(jvalue *)
  template<typename T>
  [[gnu::always_inline]]
  inline static T getAArg(jvalue *values) {
   using componentType = typename ComponentTypeResolver<T>::type;
   static_assert(
    JValueArgResolver<false, componentType>::isRegisteredResolver
    || JValueArgResolver<true, componentType>::isRegisteredResolver,
    "Illegal JNI function parameter type!"
   );
   return JValueArgResolver<__is_class(componentType), T>::getAArg(values);
  }

  //Type-to-member-name maps for the jvalue union
  _GET_AARG_MAP(JBoolean, z)
  _GET_AARG_MAP(JByte, b)
  _GET_AARG_MAP(JChar, c)
  _GET_AARG_MAP(JShort, s)
  _GET_AARG_MAP(JInt, i)
  _GET_AARG_MAP(JLong, j)
  _GET_AARG_MAP(JFloat, f)
  _GET_AARG_MAP(JDouble, d)

  template<
   typename T,
   bool IsPointer = CX::IsPointer<T>::value,
   bool IsClass = __is_class(T),
   bool LargerThanInt = (sizeof(T) > sizeof(int))
  >
  class VArgResolver;

  //VArgResolver for pointer types
  template<typename T>
  class VArgResolver<T*, true, false, (sizeof(T*) > sizeof(int))> {
  public:
   [[gnu::always_inline]]
   inline static T* getVArg(va_list list) {
    return va_arg(list, T*);
   }
  };

  //VArgResolver for integral types smaller than int
  //Promotes to int and lossy-casts
  template<typename T>
  class VArgResolver<T, false, false, false> {
  public:
   [[gnu::always_inline]]
   inline static T getVArg(va_list list) {
    return (T)va_arg(list, int);
   }
  };

  //VArgResolver for integral types larger than int
  //Promotes to double and lossy-casts
  template<typename T>
  class VArgResolver<T, false, false, true> {
  public:
   [[gnu::always_inline]]
   inline static T getVArg(va_list list) {
    return (T)va_arg(list, double);
   }
  };

  //VArgResolver for object types
  //Consuming an object type off of a va_list is undefined behaviour
  template<typename T>
  class VArgResolver<T, false, true> {
  public:
   [[gnu::always_inline]]
   inline static T getVArg(va_list list) {
    //Static assertion will always fail in fault-conditions, uses sfinae to prevent
    //compiler errors in non-fault conditions, since static_assert(false, ...); will
    //always throw an error, even if the template is not instantiated
    static_assert(
     !__is_class(T),
     "Consuming an object type off of a va_list is undefined behaviour!\n"
     "Did you intend to consume a pointer-to-object type?"
    );
   }
  };

  template<typename T>
  [[gnu::always_inline]]
  inline static T getVArg(va_list args) {
   return VArgResolver<T>::getVArg(args);
  }

  template<auto, typename...>
  class FunctionAccessor;

  //Entry invoker for member functions
  template<auto N, typename T, typename R, typename... Args>
  class FunctionAccessor<N, R (T::* const)(Args...)> {
  public:
   using argType = typename CX::TemplateTypeIterator<N - 1, Args...>::type;
   using functionType = R (T::* const)(Args...);
   using erasedType = void (AnyClass::* const)();

   template<typename... DecomposedVarargs>
   [[gnu::always_inline]]
   inline static R invokeV(void * const inst, erasedType func, va_list list, DecomposedVarargs... args) {
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
   inline static R invokeA(void * const inst, erasedType func, jvalue * const values, DecomposedJValues... args) {
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
    va_end(list);
    return invokeA(inst, func, nullptr, args...);
   }

   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeA(void * const inst, erasedType func, jvalue *values, Args2... args) {
    static_assert(
     CX::IsSame<std::tuple<Args...>, std::tuple<Args2...>>::value,
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
   inline static R invokeA(erasedType func, jvalue * const values, DecomposedJValues... args) {
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
    va_end(list);
    return invokeA(func, nullptr, args...);
   }

   template<typename... Args2>
   inline static R invokeA(erasedType func, jvalue * const values, Args2... args) {
    static_assert(
     CX::IsSame<std::tuple<Args...>, std::tuple<Args2...>>::value,
     "Function argument list does not match base invoker arguments!"
    );
    return ((R (*)(Args...))func)(args...);
   }
  };

  //SFINAE functions to generate JNI function signatures
  //Fold expression evaluators
  //Evaluator for non-empty fold
  template<typename...>
  struct EvaluateFold;

  template<typename T, typename... Args>
  struct EvaluateFold<T, Args...> {
   static constexpr const auto fold = CX::Concat<JniTypeBase<T>::signature, EvaluateFold<Args...>::fold>::result;
  };

  //Evaluator for empty folds
  template<>
  struct EvaluateFold<> {
   static constexpr const char fold[] = "";
  };

  template<bool IsConstructor, typename R, typename... Args>
  struct SignatureGenerator;

  //Generator for regular functions
  template<typename R, typename... Args>
  struct SignatureGenerator<false, R, Args...> {
   static constexpr const char
    prefix[] = "(",
    suffix[] = ")";
   static constexpr const auto signature =
    CX::Concat<prefix, EvaluateFold<Args...>::fold, suffix, JniTypeBase<R>::signature>::result;
  };

  //Generator for constructors
  template<typename R, typename... Args>
  struct SignatureGenerator<true, R, Args...> {
   static constexpr const char suffix[] = ")V";
   static constexpr const auto signature =
    CX::Concat<SignatureGenerator<false, R, Args...>::prefix, EvaluateFold<Args...>::fold, suffix>::result;
  };
 }

 template<auto F, typename T = decltype(F), auto = CX::IsFunction<T>::value>
 struct Function;

 template<auto F>
 struct Function<F, decltype(F), nullptr> {
  static constexpr const auto function = F;

  constexpr Function() noexcept = default;
 };

 template<auto F, typename T, typename R, typename... Args>
 struct Function<F, R (T::*)(Args...), true> : Function<F, decltype(F), nullptr> {
  constexpr Function() noexcept {
   static_assert(
    !CX::IsVirtualFunction<F>::value,
    "Virtual functions cannot be registered with fake-jni! If you intend to register an overload for a native base "
    "type, you must register a function with the exact same name and signature as the target function in the base."
   );
  }
 };

 template<auto F, typename R, typename... Args>
 struct Function<F, R (*)(Args...), true> : Function<F, decltype(F), nullptr> {
  using Function<F, decltype(F), nullptr>::Function;
 };

 template<auto F, typename R, typename... Args>
 struct Function<F, R (&)(Args...), true> : Function<F, decltype(F), nullptr> {
  using Function<F, decltype(F), nullptr>::Function;
 };

 template<auto F, typename R, typename... Args>
 struct Function<F, R (Args...), true> : Function<F, decltype(F), nullptr> {
  using Function<F, decltype(F), nullptr>::Function;
 };

 //Wrapper template for constructor detection and registration
 template<typename T, typename... Args>
 class Constructor {
 public:
  constexpr Constructor() noexcept {
   static_assert(CX::HasConstructor<T, Args...>::value, "Tried to register non-existent constructor!");
  }

  [[gnu::always_inline]]
  inline static T * construct(Args... args) {
   return new T(args...);
  }
 };
}