#include "jni.h"
#include "fake-jni/jvm.h"

#include <stdexcept>

namespace FakeJni {
//TODO implement
 jint NativeInterface::throw_(jthrowable) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::throw_' is unimplemented!");
  return 0;
 }

//TODO implement
 jint NativeInterface::throwNew(jclass, const char *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::throwNew' is unimplemented!");
  return 0;
 }

//TODO implement
 jthrowable NativeInterface::exceptionOccurred() const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::exceptionOccurred' is unimplemented!");
  return nullptr;
 }

//TODO implement
 void NativeInterface::exceptionDescribe() const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::exceptionDescribe' is unimplemented!");
 }

//TODO implement
 void NativeInterface::exceptionClear() const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::exceptionClear' is unimplemented!");
 }

//TODO implement
 jboolean NativeInterface::exceptionCheck() const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::exceptionCheck' is unimplemented!");
  return 0;
 }

//TODO implement
 void NativeInterface::fatalError(const char *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::fatalError' is unimplemented!");
 }

//TODO implement
 jint NativeInterface::pushLocalFrame(jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::pushLocalFrame' is unimplemented!");
  return 0;
 }

//TODO implement
 jobject NativeInterface::popLocalFrame(jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::popLocalFrame' is unimplemented!");
  return nullptr;
 }
}