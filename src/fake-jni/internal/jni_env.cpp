#include "jni.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
 JniEnv::JniEnv(Jvm * const vm, NativeInterface * const ni) noexcept :
  JNIEnv(),
  vm(vm),
  ni(ni)
 {
  functions = ni;
 }

 Jvm * JniEnv::getVM() {
  return vm;
 }
}