#pragma once

namespace FakeJVM {
//TODO implement
 jboolean NativeInterface::isSameObject(jobject, jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::isSameObject' is unimplemented!");
  return 0;
 }

//TODO implement
 jobject NativeInterface::allocObject(jclass) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::allocObject' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jobject NativeInterface::newObject(jclass, jmethodID, ...) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newObject' is unimplemented!");
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