#include "fake-jni/internal/jni/native.h"
#include "fake-jni/jvm.h"

#include <cx/vararg.h>

#define _BEGIN_VA_LIST \
CX::va_list_t l;\
va_start(l, jmid);

namespace FakeJni {
 jobject NativeInterface::_NewObject(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(NewObject)
  return ni->newObjectV(clazz, jmid, l);
 }

 jobject NativeInterface::_CallObjectMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallObjectMethod)
  return ni->callObjectMethodV(obj, jmid, l);
 }

 jboolean NativeInterface::_CallBooleanMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallBooleanMethod)
  return ni->callBooleanMethodV(obj, jmid, l);
 }

 jbyte NativeInterface::_CallByteMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallByteMethod)
  return ni->callByteMethodV(obj, jmid, l);
 }

 jchar NativeInterface::_CallCharMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallCharMethod)
  return ni->callCharMethodV(obj, jmid, l);
 }

 jshort NativeInterface::_CallShortMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallShortMethod)
  return ni->callShortMethodV(obj, jmid, l);
 }

 jint NativeInterface::_CallIntMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallIntMethod)
  return ni->callIntMethodV(obj, jmid, l);
 }

 jlong NativeInterface::_CallLongMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallLongMethod)
  return ni->callLongMethodV(obj, jmid, l);
 }

 jfloat NativeInterface::_CallFloatMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallFloatMethod)
  return ni->callFloatMethodV(obj, jmid, l);
 }

 jdouble NativeInterface::_CallDoubleMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallDoubleMethod)
  return ni->callDoubleMethodV(obj, jmid, l);
 }

 void NativeInterface::_CallVoidMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallVoidMethod)
  ni->callVoidMethodV(obj, jmid, l);
 }

 jobject NativeInterface::_CallNonvirtualObjectMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallNonvirtualObjectMethod)
  return ni->callNonvirtualObjectMethodV(obj, clazz, jmid, l);
 }

 jboolean NativeInterface::_CallNonvirtualBooleanMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallNonvirtualBooleanMethod)
  return ni->callNonvirtualBooleanMethodV(obj, clazz, jmid, l);
 }

 jbyte NativeInterface::_CallNonvirtualByteMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallNonvirtualByteMethod)
  return ni->callNonvirtualByteMethodV(obj, clazz, jmid, l);
 }

 jchar NativeInterface::_CallNonvirtualCharMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallNonvirtualCharMethod)
  return ni->callNonvirtualCharMethodV(obj, clazz, jmid, l);
 }

 jshort NativeInterface::_CallNonvirtualShortMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallNonvirtualShortMethod)
  return ni->callNonvirtualShortMethodV(obj, clazz, jmid, l);
 }

 jint NativeInterface::_CallNonvirtualIntMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallNonvirtualIntMethod)
  return ni->callNonvirtualIntMethodV(obj, clazz, jmid, l);
 }

 jlong NativeInterface::_CallNonvirtualLongMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallNonvirtualLongMethod)
  return ni->callNonvirtualLongMethodV(obj, clazz, jmid, l);
 }

 jfloat NativeInterface::_CallNonvirtualFloatMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallNonvirtualFloatMethod)
  return ni->callNonvirtualFloatMethodV(obj, clazz, jmid, l);
 }

 jdouble NativeInterface::_CallNonvirtualDoubleMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallNonvirtualDoubleMethod)
  return ni->callNonvirtualDoubleMethodV(obj, clazz, jmid, l);
 }

 void NativeInterface::_CallNonvirtualVoidMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallNonvirtualVoidMethodV)
  ni->callNonvirtualVoidMethodV(obj, clazz, jmid, l);
 }

 jobject NativeInterface::_CallStaticObjectMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallStaticObjectMethod)
  return ni->callStaticObjectMethodV(clazz, jmid, l);
 }

 jboolean NativeInterface::_CallStaticBooleanMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallStaticBooleanMethod)
  return ni->callStaticBooleanMethodV(clazz, jmid, l);
 }

 jbyte NativeInterface::_CallStaticByteMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallStaticByteMethod)
  return ni->callStaticByteMethodV(clazz, jmid, l);
 }

 jchar NativeInterface::_CallStaticCharMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallStaticCharMethod)
  return ni->callStaticCharMethodV(clazz, jmid, l);
 }

 jshort NativeInterface::_CallStaticShortMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallShortMethod)
  return ni->callStaticShortMethodV(clazz, jmid, l);
 }

 jint NativeInterface::_CallStaticIntMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallStaticIntMethod)
  return ni->callStaticIntMethodV(clazz, jmid, l);
 }

 jlong NativeInterface::_CallStaticLongMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallStaticLongMethod)
  return ni->callStaticLongMethodV(clazz, jmid, l);
 }

 jfloat NativeInterface::_CallStaticFloatMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallStaticFloatMethod)
  return ni->callStaticFloatMethodV(clazz, jmid, l);
 }

 jdouble NativeInterface::_CallStaticDoubleMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallStaticDoubleMethod)
  return ni->callStaticDoubleMethodV(clazz, jmid, l);
 }

 void NativeInterface::_CallStaticVoidMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
  _NATIVE_DEBUG(CallStaticVoidMethod)
  return ni->callStaticVoidMethodV(clazz, jmid, l);
 }
}
