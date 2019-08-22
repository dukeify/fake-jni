#include "jni.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
 //nullptr is the default JObject instance descriptor
 const JClass * JObject::getClass() noexcept {
  return &descriptor;
 }
}

//TODO if any java.lang.Object functions from the Java STL ever need to be included, they would be declared here
DEFINE_NATIVE_DESCRIPTOR(FakeJni::JObject) {};