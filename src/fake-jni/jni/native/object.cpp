#include "jni.h"
#include "fake-jni/jvm.h"

#include <stdexcept>

namespace FakeJni {
 jboolean NativeInterface::isSameObject(jobject obj1, jobject obj2) const {
  return (jboolean)(obj1 == obj2);
 }

 jobject NativeInterface::allocObject(jclass clazz) const {
  return nullptr;
 }

//TODO implement
 jobject NativeInterface::newObjectV(jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newObjectV' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jobject NativeInterface::newObjectA(jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newObjectA' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jclass NativeInterface::getObjectClass(jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getObjectClass' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jboolean NativeInterface::isInstanceOf(jobject, jclass) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::isInstanceOf' is unimplemented!");
  return 0;
 }

//TODO implement
 jclass NativeInterface::defineClass(const char *, jobject, const jbyte *, jsize) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::defineClass' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jclass NativeInterface::findClass(const char *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::findClass' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jclass NativeInterface::getSuperclass(jclass) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getSuperclass' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jboolean NativeInterface::isAssignableFrom(jclass, jclass) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::isAssignableFrom' is unimplemented!");
  return 0;
 }


}