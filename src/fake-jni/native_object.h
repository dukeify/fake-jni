#pragma once

#define DEFINE_CLASS_NAME(str) \
inline static constexpr const char name[] = str;\
static const JClass descriptor;\
const JClass * getDescriptor() noexcept override {\
 return &descriptor;\
}

#define DEFINE_NATIVE_TYPE(clazz) \
namespace FakeJni::_CX {\
 template<>\
 class JniTypeBase<clazz> {\
 private:\
  inline static constexpr const char \
   prefix[] = "L",\
   suffix[] = ";";\
 public:\
  inline static constexpr const bool isRegisteredType = true;\
  inline static constexpr const auto signature = CX::Concat<prefix, clazz::name, suffix>::result;\
 };\
}\
inline const FakeJni::JClass clazz::descriptor

namespace FakeJni {
//TODO detect if function is virtual: if any derived classes, of previously defined native classes, contain
// a virtual override for the base, then register a new java function association for the derived class and
// the respective function signature and classpath
 template<typename T>
 class NativeObject: public JClass {
 private:
  inline static constexpr const auto deallocator = &_CX::Deallocator<T>::deallocate;

 public:
  explicit NativeObject(NativeObject &) = delete;
  explicit NativeObject(NativeObject &&) = delete;

  NativeObject() noexcept : JClass(getClass()) {
   static_assert(std::is_base_of<NativeObject, T>::value, "T must be derived from NativeObject<T>!");
  }

  ~NativeObject() override = default;

  const char * getName() noexcept final {
   return T::name;
  }

  virtual const JClass * getDescriptor() = 0;

  const JClass * getClass() noexcept final {
   return getDescriptor();
  }

  JObject * newInstance(JavaVM * const vm, const char * const signature, va_list list) const noexcept final {
   Jvm * const jvm = (Jvm * const)vm;
   for (uint32_t i = 0; i < functions->getSize(); i++) {
    JMethodID * const method = (*functions)[i];
    if (strcmp(method->getSignature(), signature) == 0 && strcmp(method->getName(), "<init>") == 0) {
     return jvm->getInstances()->pushAlloc(deallocator, (JObject *)method->invoke<T *>(nullptr, list));
    }
   }
   return nullptr;
  }
 };
}