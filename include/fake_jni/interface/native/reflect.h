#pragma once

namespace FakeJni {
//TODO implement
 jmethodID NativeInterface::fromReflectedMethod(jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::fromReflectedMethod' is unimplemented!");
  return 0;
 }

//TODO implement
 jfieldID NativeInterface::fromReflectedField(jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::fromReflectedField' is unimplemented!");
  return 0;
 }

//TODO implement
 jobject NativeInterface::toReflectedMethod(jclass, jmethodID, jboolean) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::toReflectedMethod' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jobject NativeInterface::toReflectedField(jclass, jfieldID, jboolean) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::toReflectedField' is unimplemented!");
  return nullptr;
 }
}