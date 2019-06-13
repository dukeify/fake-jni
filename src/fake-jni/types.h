#pragma once

#define DEFINE_JNI_TYPE(target, sig) \
namespace _CX {\
 template<>\
 class JniTypeBase<typename ComponentTypeResolver<target>::type> {\
 public:\
  inline static constexpr const bool isRegisteredType = true;\
  inline static constexpr const char signature[] = #sig;\
 };\
}

namespace FakeJni {
 namespace _CX {
  //JNI Type metadata template
  template<typename>
  class JniTypeBase {
  public:
   inline static constexpr const bool isRegisteredType = false;
   inline static constexpr const char signature[] = "[INVALID_TYPE]";
  };

  //Strip pointers off of JniTypeBase specializations and instantiations
  template<typename T>
  class JniTypeBase<T*> {
  private:
   using type = JniTypeBase<typename ComponentTypeResolver<T*>::type>;
  public:
   inline static constexpr const auto isRegisteredType = type::isRegisteredType;
   inline static constexpr const auto signature = type::signature;
  };

  //Strip references off of JniTypeBase specializations and instantiations
  template<typename T>
  class JniTypeBase<T&> {
  private:
   using type = JniTypeBase<typename ComponentTypeResolver<T&>::type>;
  public:
   inline static constexpr const auto isRegisteredType = type::isRegisteredType;
   inline static constexpr const auto signature = type::signature;
  };

  //Strip const qualifications off of JniTypeBase specializations and instantiations
  template<typename T>
  class JniTypeBase<const T> {
  private:
   using type = JniTypeBase<typename ComponentTypeResolver<const T>::type>;
  public:
   inline static constexpr const auto isRegisteredType = type::isRegisteredType;
   inline static constexpr const auto signature = type::signature;
  };

  //Ensures that all type arguments are valid JNI parameters
  template<typename...>
  class VerifyJNIArguments;

  template<typename T, typename... Args>
  class VerifyJNIArguments<T, Args...> {
  public:
   [[gnu::always_inline]]
   inline static constexpr bool verify() {
    return VerifyJNIArguments<T>::verify() && VerifyJNIArguments<Args...>::verify();
   }
  };

  template<typename T>
  class VerifyJNIArguments<T> {
  public:
   [[gnu::always_inline]]
   inline static constexpr bool verify() {
    using resolver = ComponentTypeResolver<T>;
    if constexpr(std::is_class<typename resolver::type>::value) {
     return std::is_base_of<_jobject, typename resolver::type>::value && resolver::indirectionCount == 1U;
    } else {
     return JniTypeBase<T>::isRegisteredType && resolver::indirectionCount == 0U;
    }
   }
  };

  template<>
  class VerifyJNIArguments<> {
  public:
   [[gnu::always_inline]]
   inline static constexpr bool verify() {
    return true;
   }
  };

  //Explicit downcasting generators to handle multiple inheritance from JNI types
  template<typename...>
  class DowncastingGenerator;

  template<typename Target, typename Step, typename... Steps>
  class DowncastingGenerator<Target, Step, Steps...> {
  private:
   using target_t = typename ComponentTypeResolver<Target>::type *;
   using step_t = typename ComponentTypeResolver<Step>::type *;
   using arg_t = typename ComponentTypeResolver<typename CX::TemplateTypeIterator<0, Steps...>::type>::type *;

  public:
   [[gnu::always_inline]]
   inline static target_t cast(step_t step) {
    return DowncastingGenerator<Target, Steps...>::cast((arg_t)step);
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

 class Jvm;

 class InvokeInterface;
 class NativeInterface;

 class Library;
 class LibraryOptions;

 class JniEnv: public JNIEnv {
 public:
  Jvm * const vm;

 public:
  JniEnv(Jvm * const vm) noexcept :
   JNIEnv(),
   vm(vm)
  {}

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
  {}

  virtual Jvm * getVM() final {
   return vm;
  }
 };

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
}