#include "jni.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
 JniEnv::JniEnv(const Jvm& vm) noexcept :
  JNIEnv(),
  vm(const_cast<Jvm &>(vm))
 {
  functions = &vm.getNativeInterface();
 }

 const Jvm& JniEnv::getVM() const noexcept {
  return vm;
 }

 void JniEnv::setNativeInterface(FakeJni::NativeInterface &interface) noexcept {
  functions = &interface;
 }
}