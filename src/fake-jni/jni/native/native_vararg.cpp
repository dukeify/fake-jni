#include "fake-jni/jvm.h"

#define _BEGIN_VA_LIST \
va_list l;\
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
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallByteMethod\n");
#endif
  return ni->callByteMethodV(obj, jmid, l);
 }

 jchar NativeInterface::_CallCharMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallCharMethod\n");
#endif
  return ni->callCharMethodV(obj, jmid, l);
 }

 jshort NativeInterface::_CallShortMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallShortMethod\n");
#endif
  return ni->callShortMethodV(obj, jmid, l);
 }

 jint NativeInterface::_CallIntMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallIntMethod\n");
#endif
  return ni->callIntMethodV(obj, jmid, l);
 }

 jlong NativeInterface::_CallLongMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallLongMethod\n");
#endif
  return ni->callLongMethodV(obj, jmid, l);
 }

 jfloat NativeInterface::_CallFloatMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallFloatMethod\n");
#endif
  return ni->callFloatMethodV(obj, jmid, l);
 }

 jdouble NativeInterface::_CallDoubleMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallDoubleMethod\n");
#endif
  return ni->callDoubleMethodV(obj, jmid, l);
 }

 void NativeInterface::_CallVoidMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallVoidMethod\n");
#endif
  ni->callVoidMethodV(obj, jmid, l);
 }

 jobject NativeInterface::_CallNonvirtualObjectMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualObjectMethod\n");
#endif
  return ni->callNonvirtualObjectMethodV(obj, clazz, jmid, l);
 }

 jboolean NativeInterface::_CallNonvirtualBooleanMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualBooleanMethod\n");
#endif
  return ni->callNonvirtualBooleanMethodV(obj, clazz, jmid, l);
 }

 jbyte NativeInterface::_CallNonvirtualByteMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualByteMethod\n");
#endif
  return ni->callNonvirtualByteMethodV(obj, clazz, jmid, l);
 }

 jchar NativeInterface::_CallNonvirtualCharMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualCharMethod\n");
#endif
  return ni->callNonvirtualCharMethodV(obj, clazz, jmid, l);
 }

 jshort NativeInterface::_CallNonvirtualShortMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualShortMethod\n");
#endif
  return ni->callNonvirtualShortMethodV(obj, clazz, jmid, l);
 }

 jint NativeInterface::_CallNonvirtualIntMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualIntMethod\n");
#endif
  return ni->callNonvirtualIntMethodV(obj, clazz, jmid, l);
 }

 jlong NativeInterface::_CallNonvirtualLongMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualLongMethod\n");
#endif
  return ni->callNonvirtualLongMethodV(obj, clazz, jmid, l);
 }

 jfloat NativeInterface::_CallNonvirtualFloatMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualFloatMethod\n");
#endif
  return ni->callNonvirtualFloatMethodV(obj, clazz, jmid, l);
 }

 jdouble NativeInterface::_CallNonvirtualDoubleMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualDoubleMethod\n");
#endif
  return ni->callNonvirtualDoubleMethodV(obj, clazz, jmid, l);
 }

 void NativeInterface::_CallNonvirtualVoidMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualVoidMethod\n");
#endif
  ni->callNonvirtualVoidMethodV(obj, clazz, jmid, l);
 }

 jobject NativeInterface::_CallStaticObjectMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticObjectMethod\n");
#endif
  return ni->callStaticObjectMethodV(clazz, jmid, l);
 }

 jboolean NativeInterface::_CallStaticBooleanMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticBooleanMethod\n");
#endif
  return ni->callStaticBooleanMethodV(clazz, jmid, l);
 }

 jbyte NativeInterface::_CallStaticByteMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticByteMethod\n");
#endif
  return ni->callStaticByteMethodV(clazz, jmid, l);
 }

 jchar NativeInterface::_CallStaticCharMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticCharMethod\n");
#endif
  return ni->callStaticCharMethodV(clazz, jmid, l);
 }

 jshort NativeInterface::_CallStaticShortMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticShortMethod\n");
#endif
  return ni->callStaticShortMethodV(clazz, jmid, l);
 }

 jint NativeInterface::_CallStaticIntMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticIntMethod\n");
#endif
  return ni->callStaticIntMethodV(clazz, jmid, l);
 }

 jlong NativeInterface::_CallStaticLongMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticLongMethod\n");
#endif
  return ni->callStaticLongMethodV(clazz, jmid, l);
 }

 jfloat NativeInterface::_CallStaticFloatMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticFloatMethod\n");
#endif
  return ni->callStaticFloatMethodV(clazz, jmid, l);
 }

 jdouble NativeInterface::_CallStaticDoubleMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticDoubleMethod\n");
#endif
  return ni->callStaticDoubleMethodV(clazz, jmid, l);
 }

 void NativeInterface::_CallStaticVoidMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...) {
  _FETCH_JNI_ENV
  _BEGIN_VA_LIST
#ifdef FAKE_JNI_DEBUG
  fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::CallStaticVoidMethod\n");
#endif
  return ni->callStaticVoidMethodV(clazz, jmid, l);
 }
}
