#include "jni.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
 JClass::~JClass() {
  delete functions;
  delete fields;
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

 const char * JClass::getName() const noexcept {
  return className;
 }

 //By default, allocations will not be supported. This prevents attempts to instantiate `java/lang/Class` directly.
 JObject * JClass::newInstance(JavaVM * const vm, const char * const signature, va_list list) const {
  return constructV(vm, signature, list);
 }

 JObject * JClass::newInstance(JavaVM * const vm, const char * const signature, jvalue * const values) const {
  return constructA(vm, signature, values);
 }
}

//Allocate JClass descriptor for JClass
DEFINE_NATIVE_DESCRIPTOR(FakeJni::JClass)END_NATIVE_DESCRIPTOR