#include "jni.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
 JvmtiEnv::JvmtiEnv(Jvm * const vm, JvmtiInterface * const ji) noexcept :
  jvmtiEnv(),
  vm(vm),
  ji(ji)
 {
  functions = ji;
 }

 Jvm * JvmtiEnv::getVM() {
  return vm;
 }
}