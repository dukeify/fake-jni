#pragma once

#include "fake-jni/jvm.h"

//JNI java/lang/Throwable implementation
namespace FakeJni {
 //TODO this is unfinished
 class JThrowable : public JObject {
 public:
  DEFINE_CLASS_NAME("java/lang/Throwable")

  JThrowable();
 };
}

_DEFINE_JNI_CONVERSION_OPERATOR_(FakeJni::JThrowable, jthrowable)

DEFINE_JNI_TYPE(_jthrowable, "java/lang/Throwable")
DECLARE_NATIVE_TYPE(FakeJni::JThrowable)