#include "jni.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
 JvmtiEnv::JvmtiEnv(const Jvm& vm) noexcept :
  jvmtiEnv(),
  vm(vm)
 {
  functions = &vm.getJvmtiInterface();
 }

 const Jvm& JvmtiEnv::getVM() const noexcept {
  return vm;
 }

 JvmtiEnv& JvmtiEnv::operator=(const JvmtiEnv &env) noexcept {
  return const_cast<JvmtiEnv&>(env);
 }
}