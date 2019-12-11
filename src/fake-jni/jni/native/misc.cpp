#include "fake-jni/jvm.h"

#include <stdexcept>

namespace FakeJni {
//TODO implement
 jint NativeInterface::ensureLocalCapacity(jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::ensureLocalCapacity' is unimplemented!");
  return 0;
 }

 jint NativeInterface::getVersion() const {
  return JNI_VERSION_1_8;
 }

 jint NativeInterface::registerNatives(jclass jclass, const JNINativeMethod * methods, const jint numMethods) const {
  bool success = true;
  const JClass * clazz = *jclass;
  for (int i = 0; i < numMethods; i++) {
   methods += i;
   success &= clazz->registerMethod(new JMethodID(methods));
  }
  return success ? JNI_OK : -1;
 }

 jint NativeInterface::unregisterNatives(jclass const jclass) const {
  bool success = true;
  JClass * const clazz = *jclass;
  std::vector<const JMethodID *> toRemove;
  for (auto& mid : clazz->getMethods()) {
   if (mid->type == JMethodID::REGISTER_NATIVES_FUNC) {
    toRemove.push_back(mid);
   }
  }
  for (auto const mid : toRemove) {
   success &= clazz->unregisterMethod(mid);
  }
  return success ? 0 : -1;
 }

//TODO implement
 jint NativeInterface::monitorEnter(jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::monitorEnter' is unimplemented!");
  return 0;
 }

//TODO implement
 jint NativeInterface::monitorExit(jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::monitorExit' is unimplemented!");
  return 0;
 }

 jint NativeInterface::getJavaVM(JavaVM **pVm) const {
  *pVm = &vm;
  return 0;
 }
}