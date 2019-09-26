#include "fake-jni/jvm.h"

#include <cx/unsafe.h>

namespace FakeJni {
 jmethodID NativeInterface::fromReflectedMethod(jobject jobj) const {
  return CX::union_cast<JMethodID *>(jobj)();
 }

 jobject NativeInterface::toReflectedMethod(jclass, jmethodID mid, jboolean) const {
  return CX::union_cast<jobject>(mid)();
 }

 jfieldID NativeInterface::fromReflectedField(jobject jobj) const {
  return CX::union_cast<JFieldID *>(jobj)();
 }

 jobject NativeInterface::toReflectedField(jclass, jfieldID fid, jboolean) const {
  return CX::union_cast<jobject>(fid)();
 }
}