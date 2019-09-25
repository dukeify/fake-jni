#include "fake-jni/jvm.h"
#include "fake-jni/array.h"

//Allocate the descriptors for all default native arrays provided by fake-jni
DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JBooleanArray)
DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JByteArray)
DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JCharArray)
DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JShortArray)
DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JIntArray)
DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JFloatArray)
DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JLongArray)
DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JDoubleArray)
DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JObjectArray)