#include "jni.h"

#include "fake-jni/jvm.h"

namespace FakeJni {
 const JClass & JObject::getClass() const noexcept {
  return descriptor;
 }
}

//TODO if any java.lang.Object functions from the Java STL ever need to be included, they would be declared here
DEFINE_NATIVE_DESCRIPTOR(FakeJni::JObject)END_NATIVE_DESCRIPTOR