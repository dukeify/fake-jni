#pragma once

#include "fake-jni/jvm.h"

//JNI java/lang/ref/WeakReference<T> implementation
namespace FakeJni {
 //TODO this is unfinished
 class JWeak : public JObject {
 public:
  DEFINE_CLASS_NAME("java/lang/ref/WeakReference")

  JWeak();
 };
}

_DEFINE_JNI_CONVERSION_OPERATOR(FakeJni::JWeak, jweak)

DEFINE_JNI_TYPE(_jweak, "java/lang/ref/WeakReference")
DECLARE_NATIVE_TYPE(FakeJni::JWeak)