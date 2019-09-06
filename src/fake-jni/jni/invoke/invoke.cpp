#include "jni.h"

#include "fake-jni/jvm.h"

#define _FETCH_JVM Jvm * const jvm = (Jvm *)vm;

//No need to store an extra stack variable if debugging is disabled
#ifdef FAKE_JNI_DEBUG
#define _INVOKE_DEBUG(name) fprintf(jvm->getLog(), "DEBUG: JNIInvokeInterface_::%s\n", #name);
#else
#define _INVOKE_DEBUG(name)
#endif

namespace FakeJni {
 InvokeInterface::InvokeInterface(Jvm * const vm) :
  JNIInvokeInterface_(),
  vm(vm)
 {
  reserved0
   = reserved1
   = reserved2
   = nullptr;
  DestroyJavaVM = [](JavaVM *vm) -> jint {
   _FETCH_JVM
   _INVOKE_DEBUG(DestroyJavaVM)
   return jvm->getInvokeInterface()->destroyJavaVm(jvm);
  };
  AttachCurrentThread = [](JavaVM *vm, void **penv, void *args) -> jint {
   _FETCH_JVM
   _INVOKE_DEBUG(AttachCurrentThread)
   return jvm->getInvokeInterface()->attachCurrentThread(jvm, penv, args);
  };
  DetachCurrentThread = [](JavaVM *vm) -> jint {
   _FETCH_JVM
   _INVOKE_DEBUG(DetachCurrentThread)
   return jvm->getInvokeInterface()->detachCurrentThread((Jvm *)vm);
  };
  GetEnv = [](JavaVM *vm, void **penv, jint version) -> jint {
   _FETCH_JVM
   _INVOKE_DEBUG(GetEnv)
   return jvm->getInvokeInterface()->getEnv(jvm, penv, version);
  };
  AttachCurrentThreadAsDaemon = [](JavaVM *vm, void **penv, void *args) -> jint {
   _FETCH_JVM
   _INVOKE_DEBUG(DetachCurrentThread)
   return jvm->getInvokeInterface()->attachCurrentThread(jvm, penv, args);
  };
 }
}