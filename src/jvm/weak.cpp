#include "fake-jni/weak.h"

namespace FakeJni {
 JWeak::JWeak() {
  throw std::runtime_error("JWeak is unimplemented!");
 }
}

//Allocate JClass descriptor for JWeak
DEFINE_NATIVE_DESCRIPTOR(FakeJni::JWeak)END_NATIVE_DESCRIPTOR