#include "fake_jni.h"

FakeJVM FakeJVM::instance;

FakeJVM::FakeJVM() {
  vm_.functions = &invoke_;
  env_.functions = &native_;

  invoke_.DestroyJavaVM = [](JavaVM*) -> jint {
    // TODO(meme) perform clean-up operations
    return JNI_OK;
  };

  invoke_.AttachCurrentThread = [](JavaVM*, void**, void*) -> jint {
    return JNI_ERR;
  };

  invoke_.DetachCurrentThread = [](JavaVM*) -> jint {
    return JNI_ERR;
  };

  invoke_.AttachCurrentThreadAsDaemon = [](JavaVM*, void**, void*) -> jint {
    return JNI_ERR;
  };

  invoke_.GetEnv = [](JavaVM*, void** env, jint) -> jint {
    *env = instance.getFakeEnv();
    return JNI_OK;
  };
}
