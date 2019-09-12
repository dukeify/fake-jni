#include "jni.h"

#include "fake-jni/jvm.h"
#include "fake-jni/array.h"

//Allocate the descriptors for all default native arrays provided by fake-jni
_DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JBooleanArray)
_DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JByteArray)
_DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JCharArray)
_DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JShortArray)
_DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JIntArray)
_DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JFloatArray)
_DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JLongArray)
_DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JDoubleArray)
_DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JObjectArray)