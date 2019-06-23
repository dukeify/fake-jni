#pragma once

#define DEFINE_JNI_TYPE(target, sig) \
namespace _CX {\
 template<>\
 class JniTypeBase<typename ComponentTypeResolver<target>::type> {\
 private:\
  using type = typename ComponentTypeResolver<target>::type;\
 public:\
  static constexpr const bool isRegisteredType = true;\
  static constexpr const bool isClass = std::is_class<type>::value;\
  static constexpr const char signature[] = #sig;\
  static constexpr const bool hasComplexHierarchy = CastDefined<type>::value;\
 };\
}

namespace FakeJni {
 namespace _CX {
  using CX::ComponentTypeResolver;

  //Stub type for generating pointer-to-member types
  class AnyClass;

  //JNI Type metadata template
  template<typename>
  class JniTypeBase {
  public:
   static constexpr const bool isRegisteredType = false;
   static constexpr const bool isClass = false;
   static constexpr const char signature[] = "[INVALID_TYPE]";
   static constexpr const bool hasComplexHierarchy = false;
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
      CX::IsSame<T1, CX::ExplicitCastGenerator>::value,
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
   functions = (JNINativeInterface_ *)vm->getNativeInterface();
  }

  virtual ~JniEnv() = default;

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
   functions = (jvmtiInterface_1_ *)vm->getJvmtiInterface();
  }

  virtual ~JvmtiEnv() = default;

  virtual Jvm * getVM() final {
   return vm;
  }
 };
}