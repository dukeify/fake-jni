#include "jni.h"
#include "fake-jni/jvm.h"

#include <stdexcept>

namespace FakeJni {
 jboolean NativeInterface::isSameObject(jobject obj1, jobject obj2) const {
  return (jboolean)(obj1 == obj2);
 }

 //TODO implement
 jobject NativeInterface::allocObject(jclass clazz) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::allocObject' is unimplemented!");
  return nullptr;
 }

 jobject NativeInterface::newObjectV(jclass clazz, jmethodID mid, va_list args) const {
  return ((JClass *)clazz)->newInstance(const_cast<Jvm *>(&vm), ((JMethodID *)mid)->getSignature(), args);
 }

 jobject NativeInterface::newObjectA(jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JClass *)clazz)->newInstance(const_cast<Jvm *>(&vm), ((JMethodID *)mid)->getSignature(), args);
 }

 jclass NativeInterface::getObjectClass(jobject const obj) const {
  return (jclass)&((JObject *)obj)->getClass();
 }

 jboolean NativeInterface::isInstanceOf(jobject const obj, jclass const clazz) const {
  //pointer equivalence should be fine since class descriptors are invariant
  return (unsigned char)(&((JObject *)obj)->getClass() == ((JClass *)clazz));
 }

 jclass NativeInterface::defineClass(const char *, jobject, const jbyte *, jsize) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::defineClass' is currently unsupported!");
  return nullptr;
 }

 jclass NativeInterface::findClass(const char * const name) const {
  return (jclass)vm.findClass(name);
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