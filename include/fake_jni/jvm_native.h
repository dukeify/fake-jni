#pragma once

#include "fake_jni/types.h"
#include "fake_jni/jvm.h"

#ifdef FAKE_JNI_DEBUG
#include <cstdio>
#endif

namespace FakeJVM {
 //TODO implement in the same manner as jvm_invoke.h
 class NativeInterface: public JNINativeInterface_ {

 };
}