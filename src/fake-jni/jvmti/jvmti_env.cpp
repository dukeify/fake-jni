#include "jni.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
 JvmtiEnv::JvmtiEnv(const Jvm& vm) noexcept :
  jvmtiEnv(),
  vm(const_cast<Jvm &>(vm))
 {
  functions = &vm.getJvmtiInterface();
 }

 const Jvm& JvmtiEnv::getVM() const noexcept {
  return vm;
 }

 void JvmtiEnv::setJvmtiInterface(JvmtiInterface& interface) noexcept {
  functions = &interface;
 }
}