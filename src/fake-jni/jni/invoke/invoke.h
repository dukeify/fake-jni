#pragma once

#ifdef FAKE_JNI_DEBUG
#include <cstdio>
#endif

namespace FakeJni {
 class InvokeInterface : public JNIInvokeInterface_ {
 public:
  explicit InvokeInterface() : JNIInvokeInterface_() {
   reserved0 = reserved1 = reserved2 = nullptr;
   DestroyJavaVM = [](JavaVM *vm) -> jint {
    Jvm *const jvm = (Jvm *)vm;
#ifdef FAKE_JNI_DEBUG
    fprintf(jvm->getLog(), "DEBUG: JNIInvokeInterface_::DestroyJavaVM\n");
#endif
    return jvm->getInvokeInterface()->destroyJavaVm(jvm);
   };
   AttachCurrentThread = [](JavaVM *vm, void **penv, void *args) -> jint {
    Jvm *const jvm = (Jvm *)vm;
#ifdef FAKE_JNI_DEBUG
    fprintf(jvm->getLog(), "DEBUG: JNIInvokeInterface_::AttachCurrentThread\n");
#endif
    return jvm->getInvokeInterface()->attachCurrentThread(jvm, penv, args);
   };
   DetachCurrentThread = [](JavaVM *vm) -> jint {
    Jvm *const jvm = (Jvm *)vm;
#ifdef FAKE_JNI_DEBUG
    fprintf(jvm->getLog(), "DEBUG: JNIInvokeInterface_::DetachCurrentThread\n");
#endif
    return jvm->getInvokeInterface()->detachCurrentThread((Jvm *)vm);
   };
   GetEnv = [](JavaVM *vm, void **penv, jint version) -> jint {
    Jvm *const jvm = (Jvm *)vm;
#ifdef FAKE_JNI_DEBUG
    fprintf(jvm->getLog(), "DEBUG: JNIInvokeInterface_::GetEnv\n");
#endif
    return jvm->getInvokeInterface()->getEnv(jvm, penv, version);
   };
   AttachCurrentThreadAsDaemon = [](JavaVM *vm, void **penv, void *args) -> jint {
    Jvm *const jvm = (Jvm *)vm;
#ifdef FAKE_JNI_DEBUG
    fprintf(jvm->getLog(), "DEBUG: JNIInvokeInterface_::DetachCurrentThread\n");
#endif
    return jvm->getInvokeInterface()->attachCurrentThread(jvm, penv, args);
   };
  }

  InvokeInterface(const InvokeInterface &ii) = delete;

  virtual ~InvokeInterface() = default;

  virtual jint getEnv(Jvm *vm, void **penv, jint version) const {
   *penv = (void *)((JNIEnv *)vm->getEnv());
   return JNI_OK;
  }

  virtual jint destroyJavaVm(Jvm *vm) const {
   vm->destroy();
   return JNI_OK;
  }

  //thread.h
  virtual jint attachCurrentThread(Jvm *vm, void **penv, void *args) const;
  virtual jint detachCurrentThread(Jvm *vm) const;
  virtual jint attachCurrentThreadAsDaemon(Jvm *vm, void **penv, void *args) const;
 };
}