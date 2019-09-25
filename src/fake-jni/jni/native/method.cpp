#include "fake-jni/jvm.h"

namespace FakeJni {
 jmethodID NativeInterface::getMethodID(jclass const jclazz, const char * const name, const char * const sig) const {
  return const_cast<JMethodID *>(((JClass *)*jclazz)->getMethod(sig, name));
 }

 jobject NativeInterface::callObjectMethodV(jobject const obj, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jobject>(&vm, obj, list);
 }

 jobject NativeInterface::callObjectMethodA(jobject const obj, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jobject>(&vm, obj, args);
 }

 jboolean NativeInterface::callBooleanMethodV(jobject const obj, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jboolean>(&vm, obj, list);
 }

 jboolean NativeInterface::callBooleanMethodA(jobject const obj, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jboolean>(&vm, obj, args);
 }

 jbyte NativeInterface::callByteMethodV(jobject const obj, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jbyte>(&vm, obj, list);
 }

 jbyte NativeInterface::callByteMethodA(jobject const obj, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jbyte>(&vm, obj, args);
 }

 jchar NativeInterface::callCharMethodV(jobject const obj, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jchar>(&vm, obj, list);
 }

 jchar NativeInterface::callCharMethodA(jobject const obj, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jchar>(&vm, obj, args);
 }

 jshort NativeInterface::callShortMethodV(jobject const obj, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jshort>(&vm, obj, list);
 }

 jshort NativeInterface::callShortMethodA(jobject const obj, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jshort>(&vm, obj, args);
 }

 jint NativeInterface::callIntMethodV(jobject const obj, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jint>(&vm, obj, list);
 }

 jint NativeInterface::callIntMethodA(jobject const obj, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jint>(&vm, obj, args);
 }

 jlong NativeInterface::callLongMethodV(jobject const obj, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jlong>(&vm, obj, list);
 }

 jlong NativeInterface::callLongMethodA(jobject const obj, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jlong>(&vm, obj, args);
 }

 jfloat NativeInterface::callFloatMethodV(jobject const obj, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jfloat>(&vm, obj, list);
 }

 jfloat NativeInterface::callFloatMethodA(jobject const obj, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jfloat>(&vm, obj, args);
 }

 jdouble NativeInterface::callDoubleMethodV(jobject const obj, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jdouble>(&vm, obj, list);
 }

 jdouble NativeInterface::callDoubleMethodA(jobject const obj, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jdouble>(&vm, obj, args);
 }

 void NativeInterface::callVoidMethodV(jobject const obj, jmethodID const mid, va_list list) const {
  ((JMethodID *)mid)->invoke<void>(&vm, obj, list);
 }

 void NativeInterface::callVoidMethodA(jobject const obj, jmethodID const mid, const jvalue * const args) const {
  ((JMethodID *)mid)->invoke<void>(&vm, obj, args);
 }

 jobject NativeInterface::callNonvirtualObjectMethodV(jobject const obj, jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jobject>(&vm, (JClass *)*clazz, obj, list);
 }

 jobject NativeInterface::callNonvirtualObjectMethodA(jobject const obj, jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jobject>(&vm, (JClass *)*clazz, obj, args);
 }

 jboolean NativeInterface::callNonvirtualBooleanMethodV(jobject const obj, jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jboolean>(&vm, (JClass *)*clazz, obj, list);
 }

 jboolean NativeInterface::callNonvirtualBooleanMethodA(jobject const obj, jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jboolean>(&vm, (JClass *)*clazz, obj, args);
 }

 jbyte NativeInterface::callNonvirtualByteMethodV(jobject const obj, jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jbyte>(&vm, (JClass *)*clazz, obj, list);
 }

 jbyte NativeInterface::callNonvirtualByteMethodA(jobject const obj, jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jbyte>(&vm, (JClass *)*clazz, obj, args);
 }

 jchar NativeInterface::callNonvirtualCharMethodV(jobject const obj, jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jchar>(&vm, (JClass *)*clazz, obj, list);
 }

 jchar NativeInterface::callNonvirtualCharMethodA(jobject const obj, jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jchar>(&vm, (JClass *)*clazz, obj, args);
 }

 jshort NativeInterface::callNonvirtualShortMethodV(jobject const obj, jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jshort>(&vm, (JClass *)*clazz, obj, list);
 }

 jshort NativeInterface::callNonvirtualShortMethodA(jobject const obj, jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jshort>(&vm, (JClass *)*clazz, obj, args);
 }

 jint NativeInterface::callNonvirtualIntMethodV(jobject const obj, jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jint>(&vm, (JClass *)*clazz, obj, list);
 }

 jint NativeInterface::callNonvirtualIntMethodA(jobject const obj, jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jint>(&vm, (JClass *)*clazz, obj, args);
 }

 jlong NativeInterface::callNonvirtualLongMethodV(jobject const obj, jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jlong>(&vm, (JClass *)*clazz, obj, list);
 }

 jlong NativeInterface::callNonvirtualLongMethodA(jobject const obj, jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jlong>(&vm, (JClass *)*clazz, obj, args);
 }

 jfloat NativeInterface::callNonvirtualFloatMethodV(jobject const obj, jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jfloat>(&vm, (JClass *)*clazz, obj, list);
 }

 jfloat NativeInterface::callNonvirtualFloatMethodA(jobject const obj, jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jfloat>(&vm, (JClass *)*clazz, obj, args);
 }

 jdouble NativeInterface::callNonvirtualDoubleMethodV(jobject const obj, jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jdouble>(&vm, (JClass *)*clazz, obj, list);
 }

 jdouble NativeInterface::callNonvirtualDoubleMethodA(jobject const obj, jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jdouble>(&vm, (JClass *)*clazz, obj, args);
 }

 void NativeInterface::callNonvirtualVoidMethodV(jobject const obj, jclass const clazz, jmethodID const mid, va_list list) const {
  ((JMethodID *)mid)->invoke<void>(&vm, (JClass *)*clazz, obj, list);
 }

 void NativeInterface::callNonvirtualVoidMethodA(jobject const obj, jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  ((JMethodID *)mid)->invoke<void>(&vm, (JClass *)*clazz, obj, args);
 }

 jmethodID NativeInterface::getStaticMethodID(jclass const clazz, const char * const name, const char * const sig) const {
  return const_cast<JMethodID *>(((JClass *)*clazz)->getMethod(sig, name));
 }

 jobject NativeInterface::callStaticObjectMethodV(jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jobject>(&vm, clazz, list);
 }

 jobject NativeInterface::callStaticObjectMethodA(jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jobject>(&vm, clazz, args);
 }

 jboolean NativeInterface::callStaticBooleanMethodV(jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jboolean>(&vm, clazz, list);
 }

 jboolean NativeInterface::callStaticBooleanMethodA(jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jboolean>(&vm, clazz, args);
 }

 jbyte NativeInterface::callStaticByteMethodV(jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jbyte>(&vm, clazz, list);
 }

 jbyte NativeInterface::callStaticByteMethodA(jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jbyte>(&vm, clazz, args);
 }

 jchar NativeInterface::callStaticCharMethodV(jclass clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jchar>(&vm, clazz, list);
 }

 jchar NativeInterface::callStaticCharMethodA(jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jchar>(&vm, clazz, args);
 }

 jshort NativeInterface::callStaticShortMethodV(jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jshort>(&vm, clazz, list);
 }

 jshort NativeInterface::callStaticShortMethodA(jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jshort>(&vm, clazz, args);
 }

 jint NativeInterface::callStaticIntMethodV(jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jint>(&vm, clazz, list);
 }

 jint NativeInterface::callStaticIntMethodA(jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jint>(&vm, clazz, args);
 }

 jlong NativeInterface::callStaticLongMethodV(jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jlong>(&vm, clazz, list);
 }

 jlong NativeInterface::callStaticLongMethodA(jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jlong>(&vm, clazz, args);
 }

 jfloat NativeInterface::callStaticFloatMethodV(jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jfloat>(&vm, clazz, list);
 }

 jfloat NativeInterface::callStaticFloatMethodA(jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jfloat>(&vm, clazz, args);
 }

 jdouble NativeInterface::callStaticDoubleMethodV(jclass const clazz, jmethodID const mid, va_list list) const {
  return ((JMethodID *)mid)->invoke<jdouble>(&vm, clazz, list);
 }

 jdouble NativeInterface::callStaticDoubleMethodA(jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  return ((JMethodID *)mid)->invoke<jdouble>(&vm, clazz, args);
 }

 void NativeInterface::callStaticVoidMethodV(jclass const clazz, jmethodID const mid, va_list list) const {
  ((JMethodID *)mid)->invoke<void>(&vm, clazz, list);
 }

 void NativeInterface::callStaticVoidMethodA(jclass const clazz, jmethodID const mid, const jvalue * const args) const {
  ((JMethodID *)mid)->invoke<void>(&vm, clazz, args);
 }
}