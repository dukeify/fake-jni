#include "fake-jni/jvm.h"
#include "fake-jni/throwable.h"

#include <stdexcept>

namespace FakeJni {
 jint NativeInterface::throw_(jthrowable jthrow) const {
  JThrowable * throwable = *jthrow;
  jthrowable current = exceptionOccurred();
  if (current) {
   throwable->addSuppressed(*current);
   vm.clearException();
  }
  vm.throwException(*throwable);
  return JNI_OK;
 }

 jint NativeInterface::throwNew(jclass jclazz, const char * message) const {
  const auto clazz = ((JClass *)*jclazz);
  if (!isAssignableFrom(jclazz, *&JThrowable::descriptor)) {
   throw std::runtime_error(
    "FATAL: Requested exception class '"
     + std::string(clazz->getName())
     + "' does not extend 'java/lang/Throwable'!"
   );
  }
  auto constructor = clazz->getMethod("Ljava/lang/String;", "<init>");
  if (!constructor) {
   throw std::runtime_error(
    "FATAL: Requested exception class '"
     + std::string(clazz->getName())
     + "' does not expose a constructor matching the prototype: '<init>(Ljava/lang/String;)'!"
   );
  }
  //clean up string eventually
  auto jstrMessage = new JString(message);
  vm.addInstance(jstrMessage);
  vm.throwException(*constructor->invoke<JThrowable *>(&vm, clazz, jstrMessage));
  return 0;
 }

 jthrowable NativeInterface::exceptionOccurred() const {
  return vm.getException();
 }

 void NativeInterface::exceptionDescribe() const {
  auto exception = vm.getException();
  if (exception) {
   ((JThrowable *)*exception)->printStackTrace();
  }
 }

 void NativeInterface::exceptionClear() const {
  JThrowable * exception = *vm.getException();
  vm.clearException();
  delete exception;
 }

 jboolean NativeInterface::exceptionCheck() const {
  return (jboolean)(vm.getException() ? JNI_TRUE : JNI_FALSE);
 }

 void NativeInterface::fatalError(const char * message) const {
  vm.fatalError(message);
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