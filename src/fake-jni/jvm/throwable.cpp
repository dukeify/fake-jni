#include "fake-jni/throwable.h"

namespace FakeJni {
 JThrowable::JThrowable() {
  throw std::runtime_error("JThrowable is unimplemented!");
 }
}

//Allocate JClass descriptor for JThrowable
DEFINE_NATIVE_DESCRIPTOR(FakeJni::JThrowable)END_NATIVE_DESCRIPTOR