#pragma once

namespace FakeJni {
 //TODO implement
 jmethodID NativeInterface::getMethodID(jclass, const char *, const char *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getMethodID' is unimplemented!");
  return 0;
 }

//TODO implement
 jobject NativeInterface::callObjectMethodV(jobject, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callObjectMethodV' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jobject NativeInterface::callObjectMethodA(jobject, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callObjectMethodA' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jboolean NativeInterface::callBooleanMethodV(jobject, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callBooleanMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jboolean NativeInterface::callBooleanMethodA(jobject, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callBooleanMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jbyte NativeInterface::callByteMethodV(jobject, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callByteMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jbyte NativeInterface::callByteMethodA(jobject, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callByteMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jchar NativeInterface::callCharMethodV(jobject, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callCharMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jchar NativeInterface::callCharMethodA(jobject, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callCharMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jshort NativeInterface::callShortMethodV(jobject, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callShortMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jshort NativeInterface::callShortMethodA(jobject, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callShortMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jint NativeInterface::callIntMethodV(jobject, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callIntMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jint NativeInterface::callIntMethodA(jobject, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callIntMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jlong NativeInterface::callLongMethodV(jobject, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callLongMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jlong NativeInterface::callLongMethodA(jobject, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callLongMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jfloat NativeInterface::callFloatMethodV(jobject, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callFloatMethodV' is unimplemented!");
  return 0.0f;
 }

//TODO implement
 jfloat NativeInterface::callFloatMethodA(jobject, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callFloatMethodA' is unimplemented!");
  return 0.0f;
 }

//TODO implement
 jdouble NativeInterface::callDoubleMethodV(jobject, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callDoubleMethodV' is unimplemented!");
  return 0.0;
 }

//TODO implement
 jdouble NativeInterface::callDoubleMethodA(jobject, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callDoubleMethodA' is unimplemented!");
  return 0.0;
 }

//TODO implement
 void NativeInterface::callVoidMethodV(jobject, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callVoidMethodV' is unimplemented!");
 }

//TODO implement
 void NativeInterface::callVoidMethodA(jobject, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callVoidMethodA' is unimplemented!");
 }

//TODO implement
 jobject NativeInterface::callNonvirtualObjectMethodV(jobject, jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualObjectMethodV' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jobject NativeInterface::callNonvirtualObjectMethodA(jobject, jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualObjectMethodA' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jboolean NativeInterface::callNonvirtualBooleanMethodV(jobject, jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualBooleanMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jboolean NativeInterface::callNonvirtualBooleanMethodA(jobject, jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualBooleanMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jbyte NativeInterface::callNonvirtualByteMethodV(jobject, jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualByteMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jbyte NativeInterface::callNonvirtualByteMethodA(jobject, jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualByteMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jchar NativeInterface::callNonvirtualCharMethodV(jobject, jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualCharMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jchar NativeInterface::callNonvirtualCharMethodA(jobject, jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualCharMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jshort NativeInterface::callNonvirtualShortMethodV(jobject, jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualShortMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jshort NativeInterface::callNonvirtualShortMethodA(jobject, jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualShortMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jint NativeInterface::callNonvirtualIntMethodV(jobject, jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualIntMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jint NativeInterface::callNonvirtualIntMethodA(jobject, jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualIntMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jlong NativeInterface::callNonvirtualLongMethodV(jobject, jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualLongMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jlong NativeInterface::callNonvirtualLongMethodA(jobject, jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualLongMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jfloat NativeInterface::callNonvirtualFloatMethodV(jobject, jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualFloatMethodV' is unimplemented!");
  return 0.0f;
 }

//TODO implement
 jfloat NativeInterface::callNonvirtualFloatMethodA(jobject, jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualFloatMethodA' is unimplemented!");
  return 0.0f;
 }

//TODO implement
 jdouble NativeInterface::callNonvirtualDoubleMethodV(jobject, jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualDoubleMethodV' is unimplemented!");
  return 0.0;
 }

//TODO implement
 jdouble NativeInterface::callNonvirtualDoubleMethodA(jobject, jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualDoubleMethodA' is unimplemented!");
  return 0.0;
 }

//TODO implement
 void NativeInterface::callNonvirtualVoidMethodV(jobject, jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualVoidMethodV' is unimplemented!");
 }

//TODO implement
 void NativeInterface::callNonvirtualVoidMethodA(jobject, jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callNonvirtualVoidMethodA' is unimplemented!");
 }

//TODO implement
 jmethodID NativeInterface::getStaticMethodID(jclass, const char *, const char *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStaticMethodID' is unimplemented!");
  return 0;
 }

//TODO implement
 jobject NativeInterface::callStaticObjectMethodV(jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticObjectMethodV' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jobject NativeInterface::callStaticObjectMethodA(jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticObjectMethodA' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jboolean NativeInterface::callStaticBooleanMethodV(jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticBooleanMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jboolean NativeInterface::callStaticBooleanMethodA(jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticBooleanMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jbyte NativeInterface::callStaticByteMethodV(jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticByteMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jbyte NativeInterface::callStaticByteMethodA(jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticByteMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jchar NativeInterface::callStaticCharMethodV(jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticCharMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jchar NativeInterface::callStaticCharMethodA(jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticCharMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jshort NativeInterface::callStaticShortMethodV(jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticShortMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jshort NativeInterface::callStaticShortMethodA(jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticShortMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jint NativeInterface::callStaticIntMethodV(jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticIntMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jint NativeInterface::callStaticIntMethodA(jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticIntMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jlong NativeInterface::callStaticLongMethodV(jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticLongMethodV' is unimplemented!");
  return 0;
 }

//TODO implement
 jlong NativeInterface::callStaticLongMethodA(jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticLongMethodA' is unimplemented!");
  return 0;
 }

//TODO implement
 jfloat NativeInterface::callStaticFloatMethodV(jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticFloatMethodV' is unimplemented!");
  return 0.0f;
 }

//TODO implement
 jfloat NativeInterface::callStaticFloatMethodA(jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticFloatMethodA' is unimplemented!");
  return 0.0f;
 }

//TODO implement
 jdouble NativeInterface::callStaticDoubleMethodV(jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticDoubleMethodV' is unimplemented!");
  return 0.0;
 }

//TODO implement
 jdouble NativeInterface::callStaticDoubleMethodA(jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticDoubleMethodA' is unimplemented!");
  return 0.0;
 }

//TODO implement
 void NativeInterface::callStaticVoidMethodV(jclass, jmethodID, va_list) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticVoidMethodV' is unimplemented!");
 }

//TODO implement
 void NativeInterface::callStaticVoidMethodA(jclass, jmethodID, const jvalue *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::callStaticVoidMethodA' is unimplemented!");
 }
}