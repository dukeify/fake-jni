#include "jni.h"
#include "fake-jni/jvm.h"

#include <stdexcept>

namespace FakeJni {
//TODO implement
 jobjectRefType NativeInterface::getObjectRefType(jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getObjectRefType' is unimplemented!");
  return {};
 }

 jobject NativeInterface::newGlobalRef(jobject const jobj) const {
  (const_cast<Jvm&>(vm)).addInstance((JObject *)jobj);
  return jobj;
 }

 void NativeInterface::deleteGlobalRef(jobject const jobj) const {
  (const_cast<Jvm&>(vm)).removeInstance((JObject *)jobj);
 }

//TODO implement
 void NativeInterface::deleteLocalRef(jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::deleteLocalRef' is unimplemented!");
 }

//TODO implement
 jobject NativeInterface::newLocalRef(jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newLocalRef' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jweak NativeInterface::newWeakGlobalRef(jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newWeakGlobalRef' is unimplemented!");
  return nullptr;
 }

//TODO implement
 void NativeInterface::deleteWeakGlobalRef(jweak) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::deleteWeakGlobalRef' is unimplemented!");
 }
}