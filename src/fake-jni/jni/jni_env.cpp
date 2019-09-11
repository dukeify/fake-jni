#include "jni.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
 JniEnv::JniEnv(const Jvm& vm) noexcept :
  JNIEnv(),
  vm(vm)
 {
  functions = &vm.getNativeInterface();
 }

 const Jvm& JniEnv::getVM() const noexcept {
  return vm;
 }

 JniEnv& JniEnv::operator=(const JniEnv &env) noexcept {
  return const_cast<JniEnv&>(env);
 }
}