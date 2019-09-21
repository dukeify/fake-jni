#pragma once

#include "jni.h"

#include "fake-jni/jvm.h"

//JNI java/lang/ref/WeakReference<T> implementation
namespace FakeJni {
 //TODO this is unfinished
 using JWeak = JObject;
}

//DEFINE_JNI_TYPE(_jweak, "java/lang/ref/WeakReference")