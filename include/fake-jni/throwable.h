#pragma once

#include "fake-jni/jvm.h"

//JNI java/lang/Throwable implementation
namespace FakeJni {
 //TODO this is unfinished
 using JThrowable = _jthrowable;
}

DEFINE_JNI_TYPE(_jthrowable, "java/lang/Throwable")