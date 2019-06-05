#pragma once

#include "native.h"

namespace FakeJVM {
 inline jobject NativeInterface::_NewObject(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewObject\n");
#endif
  return ni->newObject(clazz, jmid, l);
 }

 inline jobject NativeInterface::_CallObjectMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallObjectMethod\n");
#endif
  return ni->callObjectMethod(obj, jmid, l);
 }

 inline jboolean NativeInterface::_CallBooleanMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallBooleanMethod\n");
#endif
  return ni->callBooleanMethod(obj, jmid, l);
 }

 inline jbyte NativeInterface::_CallByteMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallByteMethod\n");
#endif
  return ni->callByteMethod(obj, jmid, l);
 }

 inline jchar NativeInterface::_CallCharMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallCharMethod\n");
#endif
  return ni->callCharMethod(obj, jmid, l);
 }

 inline jshort NativeInterface::_CallShortMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallShortMethod\n");
#endif
  return ni->callShortMethod(obj, jmid, l);
 }

 inline jint NativeInterface::_CallIntMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallIntMethod\n");
#endif
  return ni->callIntMethod(obj, jmid, l);
 }

 inline jlong NativeInterface::_CallLongMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallLongMethod\n");
#endif
  return ni->callLongMethod(obj, jmid, l);
 }

 inline jfloat NativeInterface::_CallFloatMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallFloatMethod\n");
#endif
  return ni->callFloatMethod(obj, jmid, l);
 }

 inline jdouble NativeInterface::_CallDoubleMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallDoubleMethod\n");
#endif
  return ni->callDoubleMethod(obj, jmid, l);
 }

 inline void NativeInterface::_CallVoidMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallVoidMethod\n");
#endif
  ni->callVoidMethod(obj, jmid, l);
 }

 inline jobject NativeInterface::_CallNonvirtualObjectMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualObjectMethod\n");
#endif
  return ni->callNonvirtualObjectMethod(obj, clazz, jmid, l);
 }

 inline jboolean NativeInterface::_CallNonvirtualBooleanMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualBooleanMethod\n");
#endif
  return ni->callNonvirtualBooleanMethod(obj, clazz, jmid, l);
 }

 inline jbyte NativeInterface::_CallNonvirtualByteMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualByteMethod\n");
#endif
  return ni->callNonvirtualByteMethod(obj, clazz, jmid, l);
 }

 inline jchar NativeInterface::_CallNonvirtualCharMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualCharMethod\n");
#endif
  return ni->callNonvirtualCharMethod(obj, clazz, jmid, l);
 }

 inline jshort NativeInterface::_CallNonvirtualShortMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualShortMethod\n");
#endif
  return ni->callNonvirtualShortMethod(obj, clazz, jmid, l);
 }

 inline jint NativeInterface::_CallNonvirtualIntMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualIntMethod\n");
#endif
  return ni->callNonvirtualIntMethod(obj, clazz, jmid, l);
 }

 inline jlong NativeInterface::_CallNonvirtualLongMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualLongMethod\n");
#endif
  return ni->callNonvirtualLongMethod(obj, clazz, jmid, l);
 }

 inline jfloat NativeInterface::_CallNonvirtualFloatMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualFloatMethod\n");
#endif
  return ni->callNonvirtualFloatMethod(obj, clazz, jmid, l);
 }

 inline jdouble NativeInterface::_CallNonvirtualDoubleMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualDoubleMethod\n");
#endif
  return ni->callNonvirtualDoubleMethod(obj, clazz, jmid, l);
 }

 inline void NativeInterface::_CallNonvirtualVoidMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualVoidMethod\n");
#endif
  ni->callNonvirtualVoidMethod(obj, clazz, jmid, l);
 }

 inline jobject NativeInterface::_CallStaticObjectMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticObjectMethod\n");
#endif
  return ni->callStaticObjectMethod(clazz, jmid, l);
 }

 inline jboolean NativeInterface::_CallStaticBooleanMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticBooleanMethod\n");
#endif
  return ni->callStaticBooleanMethod(clazz, jmid, l);
 }

 inline jbyte NativeInterface::_CallStaticByteMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticByteMethod\n");
#endif
  return ni->callStaticByteMethod(clazz, jmid, l);
 }

 inline jchar NativeInterface::_CallStaticCharMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticCharMethod\n");
#endif
  return ni->callStaticCharMethod(clazz, jmid, l);
 }

 inline jshort NativeInterface::_CallStaticShortMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticShortMethod\n");
#endif
  return ni->callStaticShortMethod(clazz, jmid, l);
 }

 inline jint NativeInterface::_CallStaticIntMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticIntMethod\n");
#endif
  return ni->callStaticIntMethod(clazz, jmid, l);
 }

 inline jlong NativeInterface::_CallStaticLongMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticLongMethod\n");
#endif
  return ni->callStaticLongMethod(clazz, jmid, l);
 }

 inline jfloat NativeInterface::_CallStaticFloatMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticFloatMethod\n");
#endif
  return ni->callStaticFloatMethod(clazz, jmid, l);
 }

 inline jdouble NativeInterface::_CallStaticDoubleMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticDoubleMethod\n");
#endif
  return ni->callStaticDoubleMethod(clazz, jmid, l);
 }

 inline void NativeInterface::_CallStaticVoidMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_ENV_
  va_list l;
#ifdef FAKE_JNI_DEBUG
  fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticVoidMethod\n");
#endif
  return ni->callStaticVoidMethod(clazz, jmid, l);
 }
}