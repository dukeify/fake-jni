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

//TODO implement
 jint NativeInterface::registerNatives(jclass, const JNINativeMethod *, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::registerNatives' is unimplemented!");
  return 0;
 }

//TODO implement
 jint NativeInterface::unregisterNatives(jclass) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::unregisterNatives' is unimplemented!");
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