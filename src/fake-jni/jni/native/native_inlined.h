#pragma once

#include "fake-jni/jni/common.h"

#define _BEGIN_VA_LIST \
va_list l;\
va_start(l, jmid);

namespace FakeJni {
 inline jobject NativeInterface::_NewObject(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::NewObject\n");
#endif
  return ni->newObjectV(clazz, jmid, l);
 }

 inline jobject NativeInterface::_CallObjectMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallObjectMethod\n");
#endif
  return ni->callObjectMethodV(obj, jmid, l);
 }

 inline jboolean NativeInterface::_CallBooleanMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallBooleanMethod\n");
#endif
  return ni->callBooleanMethodV(obj, jmid, l);
 }

 inline jbyte NativeInterface::_CallByteMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallByteMethod\n");
#endif
  return ni->callByteMethodV(obj, jmid, l);
 }

 inline jchar NativeInterface::_CallCharMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallCharMethod\n");
#endif
  return ni->callCharMethodV(obj, jmid, l);
 }

 inline jshort NativeInterface::_CallShortMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallShortMethod\n");
#endif
  return ni->callShortMethodV(obj, jmid, l);
 }

 inline jint NativeInterface::_CallIntMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallIntMethod\n");
#endif
  return ni->callIntMethodV(obj, jmid, l);
 }

 inline jlong NativeInterface::_CallLongMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallLongMethod\n");
#endif
  return ni->callLongMethodV(obj, jmid, l);
 }

 inline jfloat NativeInterface::_CallFloatMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallFloatMethod\n");
#endif
  return ni->callFloatMethodV(obj, jmid, l);
 }

 inline jdouble NativeInterface::_CallDoubleMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallDoubleMethod\n");
#endif
  return ni->callDoubleMethodV(obj, jmid, l);
 }

 inline void NativeInterface::_CallVoidMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallVoidMethod\n");
#endif
  ni->callVoidMethodV(obj, jmid, l);
 }

 inline jobject NativeInterface::_CallNonvirtualObjectMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualObjectMethod\n");
#endif
  return ni->callNonvirtualObjectMethodV(obj, clazz, jmid, l);
 }

 inline jboolean NativeInterface::_CallNonvirtualBooleanMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualBooleanMethod\n");
#endif
  return ni->callNonvirtualBooleanMethodV(obj, clazz, jmid, l);
 }

 inline jbyte NativeInterface::_CallNonvirtualByteMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualByteMethod\n");
#endif
  return ni->callNonvirtualByteMethodV(obj, clazz, jmid, l);
 }

 inline jchar NativeInterface::_CallNonvirtualCharMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualCharMethod\n");
#endif
  return ni->callNonvirtualCharMethodV(obj, clazz, jmid, l);
 }

 inline jshort NativeInterface::_CallNonvirtualShortMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualShortMethod\n");
#endif
  return ni->callNonvirtualShortMethodV(obj, clazz, jmid, l);
 }

 inline jint NativeInterface::_CallNonvirtualIntMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualIntMethod\n");
#endif
  return ni->callNonvirtualIntMethodV(obj, clazz, jmid, l);
 }

 inline jlong NativeInterface::_CallNonvirtualLongMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualLongMethod\n");
#endif
  return ni->callNonvirtualLongMethodV(obj, clazz, jmid, l);
 }

 inline jfloat NativeInterface::_CallNonvirtualFloatMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualFloatMethod\n");
#endif
  return ni->callNonvirtualFloatMethodV(obj, clazz, jmid, l);
 }

 inline jdouble NativeInterface::_CallNonvirtualDoubleMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualDoubleMethod\n");
#endif
  return ni->callNonvirtualDoubleMethodV(obj, clazz, jmid, l);
 }

 inline void NativeInterface::_CallNonvirtualVoidMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualVoidMethod\n");
#endif
  ni->callNonvirtualVoidMethodV(obj, clazz, jmid, l);
 }

 inline jobject NativeInterface::_CallStaticObjectMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticObjectMethod\n");
#endif
  return ni->callStaticObjectMethodV(clazz, jmid, l);
 }

 inline jboolean NativeInterface::_CallStaticBooleanMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticBooleanMethod\n");
#endif
  return ni->callStaticBooleanMethodV(clazz, jmid, l);
 }

 inline jbyte NativeInterface::_CallStaticByteMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticByteMethod\n");
#endif
  return ni->callStaticByteMethodV(clazz, jmid, l);
 }

 inline jchar NativeInterface::_CallStaticCharMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticCharMethod\n");
#endif
  return ni->callStaticCharMethodV(clazz, jmid, l);
 }

 inline jshort NativeInterface::_CallStaticShortMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticShortMethod\n");
#endif
  return ni->callStaticShortMethodV(clazz, jmid, l);
 }

 inline jint NativeInterface::_CallStaticIntMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticIntMethod\n");
#endif
  return ni->callStaticIntMethodV(clazz, jmid, l);
 }

 inline jlong NativeInterface::_CallStaticLongMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticLongMethod\n");
#endif
  return ni->callStaticLongMethodV(clazz, jmid, l);
 }

 inline jfloat NativeInterface::_CallStaticFloatMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticFloatMethod\n");
#endif
  return ni->callStaticFloatMethodV(clazz, jmid, l);
 }

 inline jdouble NativeInterface::_CallStaticDoubleMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticDoubleMethod\n");
#endif
  return ni->callStaticDoubleMethodV(clazz, jmid, l);
 }

 inline void NativeInterface::_CallStaticVoidMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticVoidMethod\n");
#endif
  return ni->callStaticVoidMethodV(clazz, jmid, l);
 }
}