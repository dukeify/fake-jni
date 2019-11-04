#pragma once

#include "meta.h"
#include "fake-jni/internal/util.h"

#include <cx/strings.h>
#include <cx/unsafe.h>
#include <cx/vararg.h>

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

#define _ASSERT_NON_VIRTUAL \
private:\
struct NonVirtualAssertion {\
 constexpr NonVirtualAssertion() noexcept {\
  static_assert(\
   !CX::IsVirtualFunction<F>::value,\
   "Virtual functions cannot be registered with fake-jni! If you intend to register an overload for a native base "\
   "type, you must register a function with the exact same name and signature as the target function in the base."\
  );\
 }\
};\
inline static constexpr const NonVirtualAssertion check{};

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

  template<auto, typename...>
  struct FunctionAccessor;

  //Entry invoker for member functions
  template<auto N, typename T, typename R, typename... Args>
  struct FunctionAccessor<N, R (T::* const)(Args...)> {
   using arg_t = typename CX::TemplateTypeIterator<N - 1, Args...>::type;
   using func_t = R (T::* const)(Args...);
   using erased_t = void (AnyClass::* const)();

   template<typename... DecomposedVarargs>
   [[gnu::always_inline]]
   inline static R invokeV(void * const inst, erased_t func, CX::va_list_t& list, DecomposedVarargs... args) {
    return FunctionAccessor<N - 1, func_t>::template invokeV<arg_t, DecomposedVarargs...>(
     inst,
     func,
     list,
     CX::safe_va_arg<arg_t>(list),
     args...
    );
   }

   template<typename... DecomposedJValues>
   [[gnu::always_inline]]
   inline static R invokeA(void * const inst, erased_t func, jvalue * const values, DecomposedJValues... args) {
    return FunctionAccessor<N - 1, func_t>::template invokeA<arg_t, DecomposedJValues...>(
     inst,
     func,
     values + 1,
     getAArg<arg_t>(values),
     args...
    );
   }
  };

  //Base invoker for member functions
  template<typename T, typename R, typename... Args>
  struct FunctionAccessor<0, R (T::* const)(Args...)> {
   using erasedType = void (AnyClass::* const)();
   //Template pack should match Args
   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeV(void * const inst, erasedType func, CX::va_list_t& list, Args2... args) {
    va_end(list);
    return invokeA(inst, func, nullptr, args...);
   }

   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeA(void * const inst, erasedType func, jvalue *values, Args2... args) {
    static_assert(
     CX::IsSame<CX::Dummy<Args...>, CX::Dummy<Args2...>>::value,
     "Function argument list does not match base invoker arguments!"
    );
    return (((T*)inst)->*((R (T::*)(Args...))func))(args...);
   }
  };

  //Entry invoker for non-member functions
  template<auto N, typename R, typename... Args>
  struct FunctionAccessor<N, R (* const)(Args...)> {
   using arg_t = typename CX::TemplateTypeIterator<N - 1, Args...>::type;
   using func_t = R (* const)(Args...);
   using erased_t = void (* const)();

   template<typename... DecomposedVarargs>
   [[gnu::always_inline]]
   inline static R invokeV(erased_t func, CX::va_list_t& list, DecomposedVarargs... args) {
    return FunctionAccessor<N - 1, func_t>::template invokeV<arg_t, DecomposedVarargs...>(
     func,
     list,
     CX::safe_va_arg<arg_t>(list),
     args...
    );
   }

   template<typename... DecomposedJValues>
   [[gnu::always_inline]]
   inline static R invokeA(erased_t func, jvalue * const values, DecomposedJValues... args) {
    return FunctionAccessor<N - 1, func_t>::template invokeA<arg_t, DecomposedJValues...>(
     func,
     values + 1,
     getAArg<arg_t>(values),
     args...
    );
   }
  };

  //Base invoker for non-member functions
  template<typename R, typename... Args>
  struct FunctionAccessor<0, R (* const)(Args...)> {
   using erasedType = void (* const)();

   //Template pack should match Args
   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeV(erasedType func, CX::va_list_t& list, Args2... args) {
    va_end(list);
    return invokeA(func, nullptr, args...);
   }

   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeA(erasedType func, jvalue * const values, Args2... args) {
    static_assert(
     CX::IsSame<CX::Dummy<Args...>, CX::Dummy<Args2...>>::value,
     "Function argument list does not match base invoker arguments!"
    );
    return ((R (*)(Args...))func)(args...);
   }
  };

  //Entry invoker for stl functors
  template<auto N, typename R, typename... Args>
  struct FunctionAccessor<N, std::function<R (Args...)>> {
   using arg_t = typename CX::TemplateTypeIterator<N - 1, Args...>::type;
   using func_t = std::function<R (Args...)>;
   using align_t = _CX::arbitrary_align_t<sizeof(std::function<void ()>)>;

   template<typename... DecomposedVarargs>
   [[gnu::always_inline]]
   inline static R invokeV(align_t func, CX::va_list_t& list, DecomposedVarargs... args) {
    return FunctionAccessor<N - 1, func_t>::template invokeV<arg_t, DecomposedVarargs...>(
     func,
     list,
     CX::safe_va_arg<arg_t>(list),
     args...
    );
   }

   template<typename... DecomposedJValues>
   [[gnu::always_inline]]
   inline static R invokeA(align_t func, jvalue * const values, DecomposedJValues... args) {
    return FunctionAccessor<N - 1, func_t>::template invokeA<arg_t, DecomposedJValues...>(
     func,
     values + 1,
     getAArg<arg_t>(values),
     args...
    );
   }
  };

  template<typename R>
  struct FunctionAccessor<3, std::function<R (void *, void *, void *)>> {
   using align_t = _CX::arbitrary_align_t<sizeof(std::function<void ()>)>;
   using func_t = std::function<R * (void *, void *, jvalue *)>;

   template<typename...>
   [[gnu::always_inline]]
   inline static R invokeA(align_t func, const char * signature, void * env, void * classOrInst, jvalue * const values) {
    return CX::union_cast<func_t>(func)(env, classOrInst, values);
   }

   //TODO use the new <cx/vararg.h> wrapper
   template<typename...>
   [[gnu::always_inline]]
   inline static R invokeV(align_t func, const char * signature, void * env, void * classOrInst, CX::va_list_t& list) {
    throw std::runtime_error("unimplemented!");
   }
  };

  //Base invoker for stl functors
  template<typename R, typename... Args>
  struct FunctionAccessor<0, std::function<R (Args...)>> {
   using align_t = _CX::arbitrary_align_t<sizeof(std::function<void ()>)>;
   using func_t = std::function<R (Args...)>;

   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeV(align_t func, CX::va_list_t& list, Args2... args) {
    va_end(list);
    return invokeA(func, nullptr, args...);
   }

   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeA(align_t func, jvalue * const values, Args2... args) {
    static_assert(
     CX::IsSame<CX::Dummy<Args...>, CX::Dummy<Args2...>>::value,
     "Function argument list does not match the base invoker arguments!"
    );
    return CX::union_cast<func_t>(func)(args...);
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
  _ASSERT_NON_VIRTUAL
 };

 template<auto F, typename T, typename R, typename... Args>
 struct Function<F, R (T::*)(Args...) const, true> : Function<F, decltype(F), nullptr> {
  _ASSERT_NON_VIRTUAL
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

#undef _GET_AARG_MAP
#undef _ASSERT_NON_VIRTUAL