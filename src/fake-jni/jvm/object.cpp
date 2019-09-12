#include "jni.h"

#include "fake-jni/jvm.h"

namespace FakeJni {
 const JClass & JObject::getClass() const noexcept {
  return descriptor;
 }
}

DEFINE_NATIVE_DESCRIPTOR(FakeJni::JObject)END_NATIVE_DESCRIPTOR