#include "jni.h"
#include "fake-jni/jvm.h"

#include <stdexcept>

namespace FakeJni {
//TODO implement
 jint NativeInterface::ensureLocalCapacity(jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::ensureLocalCapacity' is unimplemented!");
  return 0;
 }

//TODO implement
 jint NativeInterface::getVersion() const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getVersion' is unimplemented!");
  return 0;
 }

 //TODO throws NoSuchMethodError if a method is not native or cannot be found
 // once exceptions are implemented, add this functionality
 jint NativeInterface::registerNatives(jclass clazz, const JNINativeMethod * methods, const jint numMethods) const {
  bool success = true;
  const auto jclass = (JClass * const)clazz;
  for (int i = 0; i < numMethods; i++) {
   methods += i;
   success &= jclass->registerMethod(new JMethodID(methods));
  }
  return success ? JNI_OK : -1;
 }

 jint NativeInterface::unregisterNatives(jclass const jclass) const {
  auto * const clazz = (JClass *)jclass;
  const auto& methods = clazz->getMethods();
  std::vector<JMethodID *> toRemove;
  for (unsigned int i = 0; i < methods.getSize(); i++) {
   const auto mid = methods[i];
   if (mid->type == JMethodID::REGISTER_NATIVES_FUNC) {
    toRemove.push_back(mid);
   }
  }
  for (auto const mid : toRemove) {
   clazz->unregisterMethod(mid);
  }
  return 0;
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

//TODO implement
 jint NativeInterface::getJavaVM(JavaVM **) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getJavaVM' is unimplemented!");
  return 0;
 }
}