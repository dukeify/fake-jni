#include "jni.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
 //Private constructor for JClass descriptor
 JClass::JClass(std::nullptr_t) noexcept :
  JObject(),
  copyConstructed(nullptr),
  functions(nullptr),
  fields(nullptr)
 {}

 JClass::JClass(std::initializer_list<_CX::ClassDescriptorElement> list) noexcept :
  JObject(),
  copyConstructed(nullptr),
  functions(new AllocStack<JMethodID *>(true)),
  fields(new AllocStack<JFieldID *>(true))
 {
  for (auto &d : list) {
   d.process(this);
  }
 }

 JClass::JClass(const JClass * const clazz) noexcept :
  JObject(),
  copyConstructed(clazz),
  functions(clazz->functions),
  fields(clazz->fields)
 {}

 JClass::~JClass() {
  //The default class descriptor for JClass does not allocate any memory
  if (strcmp(getName(), "java/lang/Class") == 0 && !copyConstructed) {
   delete functions;
   delete fields;
  }
 }

 bool JClass::registerMethod(JMethodID * const mid) noexcept {
  for (uint32_t i = 0; i < functions->getSize(); i++) {
   if (*(*functions)[i] == *mid) {
    return false;
   }
  }
  functions->pushAlloc(mid);
  return true;
 }

 bool JClass::registerField(JFieldID * const fid) noexcept {
  for (uint32_t i = 0; i < fields->getSize(); i++) {
   if (*(*fields)[i] == *fid) {
    return false;
   }
  }
  fields->pushAlloc(fid);
  return true;
 }

 const JClass * JClass::getClass() noexcept {
  return const_cast<JClass *>(&descriptor);
 }

 const char * JClass::getName() noexcept {
  return "java/lang/Class";
 }

 //By default, allocations will not be supported. This prevents attempts to instantiate `java/lang/Class` directly.
 JObject * JClass::newInstance(JavaVM * const vm, const char * const signature, va_list list) const {
  return nullptr;
 }

 JObject * JClass::newInstance(JavaVM * const vm, const char * const signature, jvalue * const values) const {
  return nullptr;
 }
}

//Allocate JClass descriptor for JClass
DEFINE_NATIVE_DESCRIPTOR(FakeJni::JClass) {nullptr};