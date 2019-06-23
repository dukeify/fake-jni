#pragma once

#define DEFINE_CLASS_NAME(str) \
inline static constexpr const char name[] = str;\
static const JClass descriptor;\
inline static const JClass * getDescriptor() noexcept {\
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
  inline static constexpr const bool isClass = true;\
  inline static constexpr const auto signature = CX::Concat<prefix, clazz::name, suffix>::result;\
  inline static constexpr const bool hasComplexHierarchy = CastDefined<clazz>::value;\
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

  NativeObject() noexcept : JClass(T::getDescriptor()) {
//   static_assert(std::is_base_of<NativeObject, T>::value, "T must be derived from NativeObject<T>!");
   //Perform checks for complex hierarchy 'cast' alias member
   _CX::CastDefined<T>::assertAliasCorrectness();
  }

  ~NativeObject() override = default;

  const char * getName() noexcept final {
   return T::name;
  }

  JObject * newInstance(JavaVM * const vm, const char * const signature, va_list list) const noexcept final {
   Jvm * const jvm = (Jvm * const)vm;
   for (uint32_t i = 0; i < functions->getSize(); i++) {
    JMethodID * const method = (*functions)[i];
    if (strcmp(method->getSignature(), signature) == 0 && strcmp(method->getName(), "<init>") == 0) {
     const auto inst = method->invoke<T *>(nullptr, list);
     JObject *baseInst;
     if constexpr(_CX::JniTypeBase<T>::hasComplexHierarchy) {
      baseInst = T::cast::cast(inst);
     } else {
      baseInst = (JObject*)inst;
     }
     return jvm->getInstances()->pushAlloc(deallocator, baseInst);
    }
   }
   return nullptr;
  }
 };
}