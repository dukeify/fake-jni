#pragma once

#define DEFINE_JNI_TYPE(target, sig) \
namespace _CX {\
 template<>\
 class JniTypeBase<typename ComponentTypeResolver<target>::type> {\
 private:\
  using type = typename ComponentTypeResolver<target>::type;\
 public:\
  inline static constexpr const bool isRegisteredType = true;\
  inline static constexpr const bool isClass = std::is_class<type>::value;\
  inline static constexpr const char signature[] = #sig;\
  inline static constexpr const bool hasComplexHierarchy = CastDefined<type>::value;\
 };\
}

namespace FakeJni {
 namespace _CX {
  //JNI Type metadata template
  template<typename>
  class JniTypeBase {
  public:
   inline static constexpr const bool isRegisteredType = false;
   inline static constexpr const bool isClass = false;
   inline static constexpr const char signature[] = "[INVALID_TYPE]";
   inline static constexpr const bool hasComplexHierarchy = false;
  };

  //Strip pointers off of JniTypeBase specializations and instantiations
  template<typename T>
  class JniTypeBase<T*> : public JniTypeBase<typename ComponentTypeResolver<T*>::type> {};

  //Strip references off of JniTypeBase specializations and instantiations
  template<typename T>
  class JniTypeBase<T&> : public JniTypeBase<typename ComponentTypeResolver<T&>::type> {};

  //Strip const qualifications off of JniTypeBase specializations and instantiations
  template<typename T>
  class JniTypeBase<const T> : public JniTypeBase<typename ComponentTypeResolver<const T>::type> {};

  //Explicit casting generators to disambiguate casting with multiple inheritance JNI types
  template<typename...>
  class UpcastingGenerator;

  //JString, JCharArray, JClass, JObject
  //JObject, JString, JCharArray, JClass
  template<typename Target, typename Step, typename... Steps>
  class UpcastingGenerator<Target, Step, Steps...> {
  private:
   using target_t = typename ComponentTypeResolver<Target>::type *;
   using step_t = typename ComponentTypeResolver<Step>::type *;
   using next_t = typename ComponentTypeResolver<typename CX::TemplateTypeIterator<0U, Steps...>::type>::type *;

  public:
   [[gnu::always_inline]]
   inline static target_t cast(step_t step) {
    return UpcastingGenerator<Target, Steps...>::cast((next_t)step);
   }
  };

  template<typename Target, typename Step>
  class UpcastingGenerator<Target, Step> {
  private:
   using target_t = typename ComponentTypeResolver<Target>::type *;
   using step_t = typename ComponentTypeResolver<Step>::type *;

  public:
   [[gnu::always_inline]]
   inline static target_t cast(step_t step) {
    return (target_t)step;
   }
  };

  template<typename...>
  class DowncastingGenerator;

  template<typename Target, typename Step, typename... Steps>
  class DowncastingGenerator<Target, Step, Steps...> {
  private:
   using target_t = typename ComponentTypeResolver<Target>::type *;
   using step_t = typename ComponentTypeResolver<Step>::type *;
   using next_t = typename ComponentTypeResolver<typename CX::TemplateTypeIterator<0U, Steps...>::type>::type *;

  public:
   [[gnu::always_inline]]
   inline static target_t cast(step_t step) {
    return DowncastingGenerator<Target, Steps...>::cast((next_t)step);
   }
  };

  template<typename Target, typename Step>
  class DowncastingGenerator<Target, Step> {
  private:
   using target_t = typename ComponentTypeResolver<Target>::type *;
   using step_t = typename ComponentTypeResolver<Step>::type *;

  public:
   [[gnu::always_inline]]
   inline static target_t cast(step_t step) {
    return (target_t)step;
   }
  };

  template<typename...>
  class ExplicitCastGenerator;

  //Static assertion to prevent the compiler from spewing template errors and provide users with relevant information
  //about their incorrect usage of ExplicitCastGenerator
  template<typename Target>
  class ExplicitCastGenerator<Target> {
  private:
   class Dummy;

   using target_t = typename ComponentTypeResolver<Target>::type *;

  public:
   [[gnu::always_inline]]
   inline static target_t cast(void *) {
    //Assertion will always fail
    static_assert(
     std::is_same<Target, Dummy>::value,
     "No explicit cast is required if a polymorphic derived type is not going to be casted to a polymorphic base that "
     "is at least one type removed from the derivation. ExplicitCastGenerator is intended to be used with three or "
     "more template parameters!"
    );
   }
  };

  //Also incorrect usage, causes static assertion in ExplicitCastGenerator<typename> to trip
  template<typename T1, typename T2>
  class ExplicitCastGenerator<T1, T2> : public ExplicitCastGenerator<T1> {
  private:
   using target_t = typename ComponentTypeResolver<T1>::type *;

  public:
   [[gnu::always_inline]]
   inline static target_t cast(void *) {
    return ExplicitCastGenerator<T1>::cast(nullptr);
   }
  };

  template<typename Target, typename... Steps>
  class ExplicitCastGenerator<Target, Steps...> {
  private:
   using steps_end_t = typename CX::TemplateTypeIterator<sizeof...(Steps) - 1U, Steps...>::type;

   //Upcasting types
   using target_u_t = typename ComponentTypeResolver<steps_end_t>::type *;
   using step_u_t = typename ComponentTypeResolver<Target>::type *;

   //Downcasting types
   using target_d_t = step_u_t;
   using step_d_t = target_u_t;

   [[gnu::always_inline]]
   inline static constexpr void assertArgumentCompliance() {
    static_assert(
     std::is_class<typename ComponentTypeResolver<Target>::type>::value
      && (std::is_class<typename ComponentTypeResolver<Steps>::type>::value && ...),
     "Explicit casting routes can only be generated for polymorphic types!"
    );
   }

  public:
   using upcast_generator = typename CX::TemplateTypeArgCycler<
    0,
    sizeof...(Steps),
    UpcastingGenerator,
    Target,
    Steps...
   >::type;

   using downcast_generator = typename CX::ReverseSpecialize<
    DowncastingGenerator,
    Steps...
   >::template type<Target>;

   //Upcasting function
   [[gnu::always_inline]]
   inline static target_u_t cast(step_u_t arg) {
    assertArgumentCompliance();
    return upcast_generator::cast(arg);
   }

   //Downcasting function
   [[gnu::always_inline]]
   inline static target_d_t cast(step_d_t arg) {
    assertArgumentCompliance();
    return downcast_generator::cast(arg);
   }
  };

  //'cast' alias detection idiom
  //Negative case
  template<typename T, typename = void>
  class CastDefined : public std::false_type {
  public:
   [[gnu::always_inline]]
   inline static void assertAliasCorrectness() {}
  };

  //Positive case
  template<typename T>
  class CastDefined<T, std::void_t<typename T::cast>> : public std::true_type {
  private:
   template<typename>
   class TemplateTemplateDecomposer {};

   template<template<typename...> typename T1, typename... Args>
   class TemplateTemplateDecomposer<T1<Args...>> {
   public:
    [[gnu::always_inline]]
    inline static constexpr bool verifyParameters() {
     static_assert(
      CX::IsSame<T1, ExplicitCastGenerator>::value,
      "Illegal type for 'cast' alias, should be 'ExplicitCastGenerator<...>'!"
     );
     static_assert(
      ((JniTypeBase<Args>::isRegisteredType && JniTypeBase<Args>::isClass) && ...),
      "You may only use registered JNI / FakeJni classes in an explicit casting route!"
     );
     return true;
    }
   };

  public:
   using cast = typename T::cast;

   [[gnu::always_inline]]
   inline static void assertAliasCorrectness() {
    static_assert(
     CX::IsTemplateTemplate<cast>::value && TemplateTemplateDecomposer<cast>::verifyParameters(),
     "Illegal type for 'cast' alias, should be 'ExplicitCastGenerator<...>'!"
    );
   }
  };
 }

 using JVoid = void;
 using JBoolean = jboolean;
 using JByte = jbyte;
 using JChar = jchar;
 using JShort = jshort;
 using JInt = jint;
 using JFloat = jfloat;
 using JLong = jlong;
 using JDouble = jdouble;

 DEFINE_JNI_TYPE(JVoid, V)
 DEFINE_JNI_TYPE(JBoolean, Z)
 DEFINE_JNI_TYPE(JByte, B)
 DEFINE_JNI_TYPE(JChar, C)
 DEFINE_JNI_TYPE(JShort, S)
 DEFINE_JNI_TYPE(JInt, I)
 DEFINE_JNI_TYPE(JFloat, F)
 DEFINE_JNI_TYPE(JLong, J)
 DEFINE_JNI_TYPE(JDouble, D)

 class JObject;
 class JClass;

 class JniEnv;
 class JvmtiEnv;

 class Jvm;

 class InvokeInterface;
 class NativeInterface;
 class JvmtiInterface;

 class Library;
 class LibraryOptions;

 class Jvm: public JavaVM {
 protected:
  inline static AllocStack<Jvm *> vms;

  //TODO use proper UUID format in the future
  inline static char * generateJvmUuid() {
   static auto randCharFunc = []() -> char {
    return (char)((rand() % 2 ? 65 : 97) + (rand() % 25));
   };
   auto str = new char[32];
   for (uint32_t i = 0; i < 32; i++) {
    str[i] = randCharFunc();
   }
   //Ensure that the uuid is unique
   bool changed = true;
   uint32_t i = 0;
   while (changed && i < vms.getSize()) {
    changed = false;
    while(vms[i]->uuid == std::string(str)) {
     str[rand() % 31] = randCharFunc();
     changed = true;
    }
    i = (changed ? 0 : i + 1);
   }
   return str;
  }

 public:
  const char * uuid;

  explicit Jvm(): JavaVM(), uuid(generateJvmUuid()) {
   vms.pushAlloc(this);
  }

  virtual ~Jvm() {
   vms.removeAlloc(this);
   delete uuid;
  }

  virtual InvokeInterface * getInvokeInterface() = 0;
  virtual NativeInterface * getNativeInterface() = 0;
  virtual JvmtiInterface * getJvmtiInterface() = 0;
  virtual FILE * getLog() = 0;
  virtual JniEnv * getEnv() = 0;
  virtual void start() = 0;
  virtual void destroy() = 0;
  virtual AllocStack<JObject *>* getInstances() = 0;
  virtual Library * getLibrary(const std::string &path) = 0;
  virtual Library * attachLibrary(
   const std::string &path,
   const std::string &options,
   LibraryOptions loptions
  ) = 0;
  virtual bool removeLibrary(const std::string &path, const std::string &options) = 0;
 };

 class JniEnv: public JNIEnv {
 public:
  Jvm * const vm;

 public:
  JniEnv(Jvm * const vm) noexcept :
   JNIEnv(),
   vm(vm)
  {
   functions = (JNINativeInterface_*)vm->getInvokeInterface();
  }

  virtual Jvm * getVM() final {
   return vm;
  }
 };

 class JvmtiEnv: public jvmtiEnv {
 public:
  Jvm * const vm;

  JvmtiEnv(Jvm * const vm):
   jvmtiEnv(),
   vm(vm)
  {
   functions = (jvmtiInterface_1_*)vm->getJvmtiInterface();
  }

  virtual Jvm * getVM() final {
   return vm;
  }
 };
}