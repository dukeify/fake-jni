#pragma once

namespace FakeJni {
 //TODO implement
 jobject NativeInterface::newDirectByteBuffer(void *, jlong) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newDirectByteBuffer' is unimplemented!");
  return nullptr;
 }

//TODO implement
 void* NativeInterface::getDirectBufferAddress(jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getDirectBufferAddress' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jlong NativeInterface::getDirectBufferCapacity(jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getDirectBufferCapacity' is unimplemented!");
  return 0;
 }
}