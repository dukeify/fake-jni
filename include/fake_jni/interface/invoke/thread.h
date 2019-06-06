#pragma once

namespace FakeJni {
 jint InvokeInterface::attachCurrentThread(Jvm *vm, void **penv, void *args) const {
  throw std::runtime_error("FATAL: 'JVMInvokeInterface_::AttachCurrentThread' is unimplemented!");
  return JNI_ERR;
 }

 jint InvokeInterface::detachCurrentThread(Jvm *vm) const {
  throw std::runtime_error("FATAL: 'JVMInvokeInterface_::DetachCurrentThread' is unimplemented!");
  return JNI_ERR;
 }

 jint InvokeInterface::attachCurrentThreadAsDaemon(Jvm *vm, void **penv, void *args) const {
  throw std::runtime_error("FATAL: 'JVMInvokeInterface_::AttachCurrentThread' is unimplemented!");
  return JNI_ERR;
 }
}