#pragma once

#include "fake_jni/types.h"

namespace FakeJVM {
 NativeInterface::NativeInterface(Jvm *const vm) :
  JNINativeInterface_(),
  vm(vm)
 {
  reserved0 = reserved1 = reserved2 = reserved3 = nullptr;
  GetVersion = [](JNIEnv *env) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetVersion\n");
#endif
   return ni->getVersion();
  };
  DefineClass = [](JNIEnv *env, const char *str, jobject obj, const jbyte *bytes, jsize size) -> jclass {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::DefineClass\n");
#endif
   return ni->defineClass(str, obj, bytes, size);
  };
  FindClass = [](JNIEnv *env, const char *str) -> jclass {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::FindClass\n");
#endif
   return ni->findClass(str);
  };
  FromReflectedMethod = [](JNIEnv *env, jobject obj) -> jmethodID {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::FromReflectedMethod\n");
#endif
   return ni->fromReflectedMethod(obj);
  };
  FromReflectedField = [](JNIEnv *env, jobject obj) -> jfieldID {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::FromReflectedField\n");
#endif
   return ni->fromReflectedField(obj);
  };
  ToReflectedMethod = [](JNIEnv *env, jclass clazz, jmethodID jmid, jboolean b) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ToReflectedMethod\n");
#endif
   return ni->toReflectedMethod(clazz, jmid, b);
  };
  GetSuperclass = [](JNIEnv *env, jclass clazz) -> jclass {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetSuperclass\n");
#endif
   return ni->getSuperclass(clazz);
  };
  IsAssignableFrom = [](JNIEnv *env, jclass clazz1, jclass clazz2) -> jboolean {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::IsAssignableFrom\n");
#endif
   return ni->isAssignableFrom(clazz1, clazz2);
  };
  ToReflectedField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jboolean b) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ToReflectedField\n");
#endif
   return ni->toReflectedField(clazz, jfid, b);
  };
  Throw = [](JNIEnv *env, jthrowable t) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::Throw\n");
#endif
   return ni->throw_(t);
  };
  ThrowNew = [](JNIEnv *env, jclass clazz, const char *str) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ThrowNew\n");
#endif
   return ni->throwNew(clazz, str);
  };
  ExceptionOccurred = [](JNIEnv *env) -> jthrowable {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ExceptionOccurred\n");
#endif
   return ni->exceptionOccurred();
  };
  ExceptionDescribe = [](JNIEnv *env) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ExceptionDescribe\n");
#endif
   ni->exceptionDescribe();
  };
  ExceptionClear = [](JNIEnv *env) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ExceptionClear\n");
#endif
   ni->exceptionClear();
  };
  FatalError = [](JNIEnv *env, const char *str) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::FatalError\n");
#endif
   ni->fatalError(str);
  };
  PushLocalFrame = [](JNIEnv *env, jint i) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::PushLocalFrame\n");
#endif
   return ni->pushLocalFrame(i);
  };
  PopLocalFrame = [](JNIEnv *env, jobject obj) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::PopLocalFrame\n");
#endif
   return ni->popLocalFrame(obj);
  };
  NewGlobalRef = [](JNIEnv *env, jobject obj) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewGlobalRef\n");
#endif
   return ni->newGlobalRef(obj);
  };
  DeleteGlobalRef = [](JNIEnv *env, jobject obj) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::DeleteGlobalRef\n");
#endif
   ni->deleteGlobalRef(obj);
  };
  DeleteLocalRef = [](JNIEnv *env, jobject obj) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::DeleteLocalRef\n");
#endif
   ni->deleteLocalRef(obj);
  };
  IsSameObject = [](JNIEnv *env, jobject obj1, jobject obj2) -> jboolean {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::IsSameObject\n");
#endif
   return ni->isSameObject(obj1, obj2);
  };
  NewLocalRef = [](JNIEnv *env, jobject obj) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewLocalRef\n");
#endif
   return ni->newLocalRef(obj);
  };
  EnsureLocalCapacity = [](JNIEnv *env, jint i) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::EnsureLocalCapacity\n");
#endif
   return ni->ensureLocalCapacity(i);
  };
  AllocObject = [](JNIEnv *env, jclass clazz) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::AllocObject\n");
#endif
   return ni->allocObject(clazz);
  };
  NewObject = &NativeInterface::_NewObject;
  NewObjectV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewObjectV\n");
#endif
   return ni->newObjectV(clazz, jmid, l);
  };
  NewObjectA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *value) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewObjectA\n");
#endif
   return ni->newObjectA(clazz, jmid, value);
  };
  GetObjectClass = [](JNIEnv *env, jobject obj) -> jclass {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetObjectClass\n");
#endif
   return ni->getObjectClass(obj);
  };
  IsInstanceOf = [](JNIEnv *env, jobject obj, jclass clazz) -> jboolean {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::IsInstanceOf\n");
#endif
   return ni->isInstanceOf(obj, clazz);
  };
  GetMethodID = [](JNIEnv *env, jclass clazz, const char *str1, const char *str2) -> jmethodID {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetMethodID\n");
#endif
   return ni->getMethodID(clazz, str1, str2);
  };
  CallObjectMethod = &NativeInterface::_CallObjectMethod;
  CallObjectMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallObjectMethodV\n");
#endif
   return ni->callObjectMethodV(obj, jmid, l);
  };
  CallObjectMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallObjectMethodA\n");
#endif
   return ni->callObjectMethodA(obj, jmid, val);
  };
  CallBooleanMethod = &NativeInterface::_CallBooleanMethod;
  CallBooleanMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jboolean {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallBooleanMethodV\n");
#endif
   return ni->callBooleanMethodV(obj, jmid, l);
  };
  CallBooleanMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jboolean {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallBooleanMethodA\n");
#endif
   return ni->callBooleanMethodA(obj, jmid, val);
  };
  CallByteMethod = &NativeInterface::_CallByteMethod;
  CallByteMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jbyte {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallByteMethodV\n");
#endif
   return ni->callByteMethodV(obj, jmid, l);
  };
  CallByteMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jbyte {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallByteMethodA\n");
#endif
   return ni->callByteMethodA(obj, jmid, val);
  };
  CallCharMethod = &NativeInterface::_CallCharMethod;
  CallCharMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jchar {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallCharMethodV\n");
#endif
   return ni->callCharMethodV(obj, jmid, l);
  };
  CallCharMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jchar {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallCharMethodA\n");
#endif
   return ni->callCharMethodA(obj, jmid, val);
  };
  CallShortMethod = &NativeInterface::_CallShortMethod;
  CallShortMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jshort {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallShortMethodV\n");
#endif
   return ni->callShortMethodV(obj, jmid, l);
  };
  CallShortMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jshort {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallShortMethodA\n");
#endif
   return ni->callShortMethodA(obj, jmid, val);
  };
  CallIntMethod = &NativeInterface::_CallIntMethod;
  CallIntMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallIntMethodV\n");
#endif
   return ni->callIntMethodV(obj, jmid, l);
  };
  CallIntMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallIntMethodA\n");
#endif
   return ni->callIntMethodA(obj, jmid, val);
  };
  CallLongMethod = &NativeInterface::_CallLongMethod;
  CallLongMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jlong {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallLongMethodV\n");
#endif
   return ni->callLongMethodV(obj, jmid, l);
  };
  CallLongMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jlong {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallLongMethodA\n");
#endif
   return ni->callLongMethodA(obj, jmid, val);
  };
  CallFloatMethod = &NativeInterface::_CallFloatMethod;
  CallFloatMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jfloat {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallFloatMethodV\n");
#endif
   return ni->callFloatMethodV(obj, jmid, l);
  };
  CallFloatMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jfloat {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallFloatMethodA\n");
#endif
   return ni->callFloatMethodA(obj, jmid, val);
  };
  CallDoubleMethod = &NativeInterface::_CallDoubleMethod;
  CallDoubleMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jdouble {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallDoubleMethodV\n");
#endif
   return ni->callDoubleMethodV(obj, jmid, l);
  };
  CallDoubleMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jdouble {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallDoubleMethodA\n");
#endif
   return ni->callDoubleMethodA(obj, jmid, val);
  };
  CallVoidMethod = &NativeInterface::_CallVoidMethod;
  CallVoidMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallVoidMethodV\n");
#endif
   return ni->callVoidMethodV(obj, jmid, l);
  };
  CallVoidMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallVoidMethodA\n");
#endif
   return ni->callVoidMethodA(obj, jmid, val);
  };
  CallNonvirtualObjectMethod = &NativeInterface::_CallNonvirtualObjectMethod;
  CallNonvirtualObjectMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualObjectMethodV\n");
#endif
   return ni->callNonvirtualObjectMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualObjectMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid,
                                   const jvalue *val) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualObjectMethodA\n");
#endif
   return ni->callNonvirtualObjectMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualBooleanMethod = &NativeInterface::_CallNonvirtualBooleanMethod;
  CallNonvirtualBooleanMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jboolean {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualBooleanMethodV\n");
#endif
   return ni->callNonvirtualBooleanMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualBooleanMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid,
                                    const jvalue *val) -> jboolean {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualBooleanMethodA\n");
#endif
   return ni->callNonvirtualBooleanMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualByteMethod = &NativeInterface::_CallNonvirtualByteMethod;
  CallNonvirtualByteMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jbyte {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualByteMethodV\n");
#endif
   return ni->callNonvirtualByteMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualByteMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> jbyte {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualByteMethodA\n");
#endif
   return ni->callNonvirtualByteMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualCharMethod = &NativeInterface::_CallNonvirtualCharMethod;
  CallNonvirtualCharMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jchar {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualCharMethodV\n");
#endif
   return ni->callNonvirtualCharMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualCharMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> jchar {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualCharMethodA\n");
#endif
   return ni->callNonvirtualCharMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualShortMethod = &NativeInterface::_CallNonvirtualShortMethod;
  CallNonvirtualShortMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jshort {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualShortMethodV\n");
#endif
   return ni->callNonvirtualShortMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualShortMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid,
                                  const jvalue *val) -> jshort {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualShortMethodA\n");
#endif
   return ni->callNonvirtualShortMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualIntMethod = &NativeInterface::_CallNonvirtualIntMethod;
  CallNonvirtualIntMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualIntMethodV\n");
#endif
   return ni->callNonvirtualIntMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualIntMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualIntMethodA\n");
#endif
   return ni->callNonvirtualIntMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualLongMethod = &NativeInterface::_CallNonvirtualLongMethod;
  CallNonvirtualLongMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jlong {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualLongMethodV\n");
#endif
   return ni->callNonvirtualLongMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualLongMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> jlong {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualLongMethodA\n");
#endif
   return ni->callNonvirtualLongMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualFloatMethod = &NativeInterface::_CallNonvirtualFloatMethod;
  CallNonvirtualFloatMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jfloat {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualFloatMethodV\n");
#endif
   return ni->callNonvirtualFloatMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualFloatMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid,
                                  const jvalue *val) -> jfloat {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualFloatMethodA\n");
#endif
   return ni->callNonvirtualFloatMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualDoubleMethod = &NativeInterface::_CallNonvirtualDoubleMethod;
  CallNonvirtualDoubleMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jdouble {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualDoubleMethodV\n");
#endif
   return ni->callNonvirtualDoubleMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualDoubleMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid,
                                   const jvalue *val) -> jdouble {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualDoubleMethodA\n");
#endif
   return ni->callNonvirtualDoubleMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualVoidMethod = &NativeInterface::_CallNonvirtualVoidMethod;
  CallNonvirtualVoidMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualVoidMethodV\n");
#endif
   return ni->callNonvirtualVoidMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualVoidMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallNonvirtualVoidMethodA\n");
#endif
   return ni->callNonvirtualVoidMethodA(obj, clazz, jmid, val);
  };
  GetFieldID = [](JNIEnv *env, jclass clazz, const char *str1, const char *str2) -> jfieldID {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetFieldID\n");
#endif
   return ni->getFieldID(clazz, str1, str2);
  };
  GetObjectField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetObjectField\n");
#endif
   return ni->getObjectField(obj, jfid);
  };
  GetBooleanField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jboolean {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetBooleanField\n");
#endif
   return ni->getBooleanField(obj, jfid);
  };
  GetByteField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jbyte {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetByteField\n");
#endif
   return ni->getByteField(obj, jfid);
  };
  GetCharField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jchar {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetCharField\n");
#endif
   return ni->getCharField(obj, jfid);
  };
  GetShortField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jshort {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetShortField\n");
#endif
   return ni->getShortField(obj, jfid);
  };
  GetIntField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetIntField\n");
#endif
   return ni->getIntField(obj, jfid);
  };
  GetLongField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jlong {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetLongField\n");
#endif
   return ni->getLongField(obj, jfid);
  };
  GetFloatField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jfloat {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetFloatField\n");
#endif
   return ni->getFloatField(obj, jfid);
  };
  GetDoubleField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jdouble {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetDoubleField\n");
#endif
   return ni->getDoubleField(obj, jfid);
  };
  SetObjectField = [](JNIEnv *env, jobject obj1, jfieldID jfid, jobject obj2) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetObjectField\n");
#endif
   return ni->setObjectField(obj1, jfid, obj2);
  };
  SetBooleanField = [](JNIEnv *env, jobject obj, jfieldID jfid, jboolean b) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetBooleanField\n");
#endif
   return ni->setBooleanField(obj, jfid, b);
  };
  SetByteField = [](JNIEnv *env, jobject obj, jfieldID jfid, jbyte b) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetByteField\n");
#endif
   return ni->setByteField(obj, jfid, b);
  };
  SetCharField = [](JNIEnv *env, jobject obj, jfieldID jfid, jchar c) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetCharField\n");
#endif
   return ni->setCharField(obj, jfid, c);
  };
  SetShortField = [](JNIEnv *env, jobject obj, jfieldID jfid, jshort s) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetShortField\n");
#endif
   return ni->setShortField(obj, jfid, s);
  };
  SetIntField = [](JNIEnv *env, jobject obj, jfieldID jfid, jint i) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetIntField\n");
#endif
   return ni->setIntField(obj, jfid, i);
  };
  SetLongField = [](JNIEnv *env, jobject obj, jfieldID jfid, jlong l) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetLongField\n");
#endif
   return ni->setLongField(obj, jfid, l);
  };
  SetFloatField = [](JNIEnv *env, jobject obj, jfieldID jfid, jfloat f) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetFloatField\n");
#endif
   return ni->setFloatField(obj, jfid, f);
  };
  SetDoubleField = [](JNIEnv *env, jobject obj, jfieldID jfid, jdouble d) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetDoubleField\n");
#endif
   return ni->setDoubleField(obj, jfid, d);
  };
  GetStaticMethodID = [](JNIEnv *env, jclass clazz, const char *str1, const char *str2) -> jmethodID {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStaticMethodID\n");
#endif
   return ni->getStaticMethodID(clazz, str1, str2);
  };
  CallStaticObjectMethod = &NativeInterface::_CallStaticObjectMethod;
  CallStaticObjectMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticObjectMethodV\n");
#endif
   return ni->callStaticObjectMethodV(clazz, jmid, l);
  };
  CallStaticObjectMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticObjectMethodA\n");
#endif
   return ni->callStaticObjectMethodA(clazz, jmid, val);
  };
  CallStaticBooleanMethod = &NativeInterface::_CallStaticBooleanMethod;
  CallStaticBooleanMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jboolean {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticBooleanMethodV\n");
#endif
   return ni->callStaticBooleanMethodV(clazz, jmid, l);
  };
  CallStaticBooleanMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jboolean {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticBooleanMethodA\n");
#endif
   return ni->callStaticBooleanMethodA(clazz, jmid, val);
  };
  CallStaticByteMethod = &NativeInterface::_CallStaticByteMethod;
  CallStaticByteMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jbyte {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticByteMethodV\n");
#endif
   return ni->callStaticByteMethodV(clazz, jmid, l);
  };
  CallStaticByteMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jbyte {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticByteMethodA\n");
#endif
   return ni->callStaticByteMethodA(clazz, jmid, val);
  };
  CallStaticCharMethod = &NativeInterface::_CallStaticCharMethod;
  CallStaticCharMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jchar {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticCharMethodV\n");
#endif
   return ni->callStaticCharMethodV(clazz, jmid, l);
  };
  CallStaticCharMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jchar {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticCharMethodA\n");
#endif
   return ni->callStaticCharMethodA(clazz, jmid, val);
  };
  CallStaticShortMethod = &NativeInterface::_CallStaticShortMethod;
  CallStaticShortMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jshort {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticShortMethodV\n");
#endif
   return ni->callStaticShortMethodV(clazz, jmid, l);
  };
  CallStaticShortMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jshort {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticShortMethodA\n");
#endif
   return ni->callStaticShortMethodA(clazz, jmid, val);
  };
  CallStaticIntMethod = &NativeInterface::_CallStaticIntMethod;
  CallStaticIntMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticIntMethodV\n");
#endif
   return ni->callStaticIntMethodV(clazz, jmid, l);
  };
  CallStaticIntMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticIntMethodA\n");
#endif
   return ni->callStaticIntMethodA(clazz, jmid, val);
  };
  CallStaticLongMethod = &NativeInterface::_CallStaticLongMethod;
  CallStaticLongMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jlong {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticLongMethodV\n");
#endif
   return ni->callStaticLongMethodV(clazz, jmid, l);
  };
  CallStaticLongMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jlong {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticLongMethodA\n");
#endif
   return ni->callStaticLongMethodA(clazz, jmid, val);
  };
  CallStaticFloatMethod = &NativeInterface::_CallStaticFloatMethod;
  CallStaticFloatMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jfloat {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticFloatMethodV\n");
#endif
   return ni->callStaticFloatMethodV(clazz, jmid, l);
  };
  CallStaticFloatMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jfloat {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticFloatMethodA\n");
#endif
   return ni->callStaticFloatMethodA(clazz, jmid, val);
  };
  CallStaticDoubleMethod = &NativeInterface::_CallStaticDoubleMethod;
  CallStaticDoubleMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jdouble {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticDoubleMethodV\n");
#endif
   return ni->callStaticDoubleMethodV(clazz, jmid, l);
  };
  CallStaticDoubleMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jdouble {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticDoubleMethodA\n");
#endif
   return ni->callStaticDoubleMethodA(clazz, jmid, val);
  };
  CallStaticVoidMethod = &NativeInterface::_CallStaticVoidMethod;
  CallStaticVoidMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticVoidMethodV\n");
#endif
   return ni->callStaticVoidMethodV(clazz, jmid, l);
  };
  CallStaticVoidMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::CallStaticVoidMethodA\n");
#endif
   return ni->callStaticVoidMethodA(clazz, jmid, val);
  };
  GetStaticFieldID = [](JNIEnv *env, jclass clazz, const char *str1, const char *str2) -> jfieldID {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStaticFieldID\n");
#endif
   return ni->getStaticFieldID(clazz, str1, str2);
  };
  GetStaticObjectField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStaticObjectField\n");
#endif
   return ni->getStaticObjectField(clazz, jfid);
  };
  GetStaticBooleanField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jboolean {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStaticBooleanField\n");
#endif
   return ni->getStaticBooleanField(clazz, jfid);
  };
  GetStaticByteField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jbyte {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStaticByteField\n");
#endif
   return ni->getStaticByteField(clazz, jfid);
  };
  GetStaticCharField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jchar {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStaticCharField\n");
#endif
   return ni->getStaticCharField(clazz, jfid);
  };
  GetStaticShortField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jshort {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStaticShortField\n");
#endif
   return ni->getStaticShortField(clazz, jfid);
  };
  GetStaticIntField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStaticIntField\n");
#endif
   return ni->getStaticIntField(clazz, jfid);
  };
  GetStaticLongField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jlong {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStaticLongField\n");
#endif
   return ni->getStaticLongField(clazz, jfid);
  };
  GetStaticFloatField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jfloat {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStaticFloatField\n");
#endif
   return ni->getStaticFloatField(clazz, jfid);
  };
  GetStaticDoubleField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jdouble {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStaticDoubleField\n");
#endif
   return ni->getStaticDoubleField(clazz, jfid);
  };
  SetStaticObjectField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jobject obj) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetStaticObjectField\n");
#endif
   return ni->setStaticObjectField(clazz, jfid, obj);
  };
  SetStaticBooleanField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jboolean b) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetStaticBooleanField\n");
#endif
   return ni->setStaticBooleanField(clazz, jfid, b);
  };
  SetStaticByteField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jbyte b) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetStaticByteField\n");
#endif
   return ni->setStaticByteField(clazz, jfid, b);
  };
  SetStaticCharField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jchar c) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetStaticCharField\n");
#endif
   return ni->setStaticCharField(clazz, jfid, c);
  };
  SetStaticShortField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jshort s) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetStaticShortField\n");
#endif
   return ni->setStaticShortField(clazz, jfid, s);
  };
  SetStaticIntField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jint i) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetStaticIntField\n");
#endif
   return ni->setStaticIntField(clazz, jfid, i);
  };
  SetStaticLongField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jlong l) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetStaticLongField\n");
#endif
   return ni->setStaticLongField(clazz, jfid, l);
  };
  SetStaticFloatField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jfloat f) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetStaticFloatField\n");
#endif
   return ni->setStaticFloatField(clazz, jfid, f);
  };
  SetStaticDoubleField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jdouble d) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetStaticDoubleField\n");
#endif
   return ni->setStaticDoubleField(clazz, jfid, d);
  };
  NewString = [](JNIEnv *env, const jchar *str, jsize size) -> jstring {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewString\n");
#endif
   return ni->newString(str, size);
  };
  GetStringLength = [](JNIEnv *env, jstring str) -> jsize {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStringLength\n");
#endif
   return ni->getStringLength(str);
  };
  GetStringChars = [](JNIEnv *env, jstring str, jboolean *b) -> const jchar * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStringChars\n");
#endif
   return ni->getStringChars(str, b);
  };
  ReleaseStringChars = [](JNIEnv *env, jstring str1, const jchar *str2) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ReleaseStringChars\n");
#endif
   return ni->releaseStringChars(str1, str2);
  };
  NewStringUTF = [](JNIEnv *env, const char *str) -> jstring {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewStringUTF\n");
#endif
   return ni->newStringUTF(str);
  };
  GetStringUTFLength = [](JNIEnv *env, jstring str) -> jsize {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStringUTFLength\n");
#endif
   return ni->getStringUTFLength(str);
  };
  GetStringUTFChars = [](JNIEnv *env, jstring str, jboolean *b) -> const char * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStringUTFChars\n");
#endif
   return ni->getStringUTFChars(str, b);
  };
  ReleaseStringUTFChars = [](JNIEnv *env, jstring str1, const char *str2) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ReleaseStringUTFChars\n");
#endif
   return ni->releaseStringUTFChars(str1, str2);
  };
  GetArrayLength = [](JNIEnv *env, jarray arr) -> jsize {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetArrayLength\n");
#endif
   return ni->getArrayLength(arr);
  };
  NewObjectArray = [](JNIEnv *env, jsize size, jclass clazz, jobject obj) -> jobjectArray {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewObjectArray\n");
#endif
   return ni->newObjectArray(size, clazz, obj);
  };
  GetObjectArrayElement = [](JNIEnv *env, jobjectArray arr, jsize size) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetObjectArrayElement\n");
#endif
   return ni->getObjectArrayElement(arr, size);
  };
  SetObjectArrayElement = [](JNIEnv *env, jobjectArray arr, jsize size, jobject obj) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetObjectArrayElement\n");
#endif
   return ni->setObjectArrayElement(arr, size, obj);
  };
  NewBooleanArray = [](JNIEnv *env, jsize size) -> jbooleanArray {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewBooleanArray\n");
#endif
   return ni->newBooleanArray(size);
  };
  NewByteArray = [](JNIEnv *env, jsize size) -> jbyteArray {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewByteArray\n");
#endif
   return ni->newByteArray(size);
  };
  NewCharArray = [](JNIEnv *env, jsize size) -> jcharArray {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewCharArray\n");
#endif
   return ni->newCharArray(size);
  };
  NewShortArray = [](JNIEnv *env, jsize size) -> jshortArray {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewShortArray\n");
#endif
   return ni->newShortArray(size);
  };
  NewIntArray = [](JNIEnv *env, jsize size) -> jintArray {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewIntArray\n");
#endif
   return ni->newIntArray(size);
  };
  NewLongArray = [](JNIEnv *env, jsize size) -> jlongArray {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewLongArray\n");
#endif
   return ni->newLongArray(size);
  };
  NewFloatArray = [](JNIEnv *env, jsize size) -> jfloatArray {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewFloatArray\n");
#endif
   return ni->newFloatArray(size);
  };
  NewDoubleArray = [](JNIEnv *env, jsize size) -> jdoubleArray {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewDoubleArray\n");
#endif
   return ni->newDoubleArray(size);
  };
  GetBooleanArrayElements = [](JNIEnv *env, jbooleanArray arr, jboolean *b) -> jboolean * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetBooleanArrayElements\n");
#endif
   return ni->getBooleanArrayElements(arr, b);
  };
  GetByteArrayElements = [](JNIEnv *env, jbyteArray arr, jboolean *b) -> jbyte * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetByteArrayElements\n");
#endif
   return ni->getByteArrayElements(arr, b);
  };
  GetCharArrayElements = [](JNIEnv *env, jcharArray arr, jboolean *b) -> jchar * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetCharArrayElements\n");
#endif
   return ni->getCharArrayElements(arr, b);
  };
  GetShortArrayElements = [](JNIEnv *env, jshortArray arr, jboolean *b) -> jshort * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetShortArrayElements\n");
#endif
   return ni->getShortArrayElements(arr, b);
  };
  GetIntArrayElements = [](JNIEnv *env, jintArray arr, jboolean *b) -> jint * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetIntArrayElements\n");
#endif
   return ni->getIntArrayElements(arr, b);
  };
  GetLongArrayElements = [](JNIEnv *env, jlongArray arr, jboolean *b) -> jlong * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetLongArrayElements\n");
#endif
   return ni->getLongArrayElements(arr, b);
  };
  GetFloatArrayElements = [](JNIEnv *env, jfloatArray arr, jboolean *b) -> jfloat * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetFloatArrayElements\n");
#endif
   return ni->getFloatArrayElements(arr, b);
  };
  GetDoubleArrayElements = [](JNIEnv *env, jdoubleArray arr, jboolean *b) -> jdouble * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetDoubleArrayElements\n");
#endif
   return ni->getDoubleArrayElements(arr, b);
  };
  ReleaseBooleanArrayElements = [](JNIEnv *env, jbooleanArray arr, jboolean *b, jint i) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ReleaseBooleanArrayElements\n");
#endif
   return ni->releaseBooleanArrayElements(arr, b, i);
  };
  ReleaseByteArrayElements = [](JNIEnv *env, jbyteArray arr, jbyte *b, jint i) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ReleaseByteArrayElements\n");
#endif
   return ni->releaseByteArrayElements(arr, b, i);
  };
  ReleaseCharArrayElements = [](JNIEnv *env, jcharArray arr, jchar *c, jint i) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ReleaseCharArrayElements\n");
#endif
   return ni->releaseCharArrayElements(arr, c, i);
  };
  ReleaseShortArrayElements = [](JNIEnv *env, jshortArray arr, jshort *s, jint i) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ReleaseShortArrayElements\n");
#endif
   return ni->releaseShortArrayElements(arr, s, i);
  };
  ReleaseIntArrayElements = [](JNIEnv *env, jintArray arr, jint *i1, jint i2) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ReleaseIntArrayElements\n");
#endif
   return ni->releaseIntArrayElements(arr, i1, i2);
  };
  ReleaseLongArrayElements = [](JNIEnv *env, jlongArray arr, jlong *l, jint i) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ReleaseLongArrayElements\n");
#endif
   return ni->releaseLongArrayElements(arr, l, i);
  };
  ReleaseFloatArrayElements = [](JNIEnv *env, jfloatArray arr, jfloat *f, jint i) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ReleaseFloatArrayElements\n");
#endif
   return ni->releaseFloatArrayElements(arr, f, i);
  };
  ReleaseDoubleArrayElements = [](JNIEnv *env, jdoubleArray arr, jdouble *d, jint i) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ReleaseDoubleArrayElements\n");
#endif
   return ni->releaseDoubleArrayElements(arr, d, i);
  };
  GetBooleanArrayRegion = [](JNIEnv *env, jbooleanArray arr, jsize i1, jsize i2, jboolean *b) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetBooleanArrayRegion\n");
#endif
   return ni->getBooleanArrayRegion(arr, i1, i2, b);
  };
  GetByteArrayRegion = [](JNIEnv *env, jbyteArray arr, jsize i1, jsize i2, jbyte *b) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetByteArrayRegion\n");
#endif
   return ni->getByteArrayRegion(arr, i1, i2, b);
  };
  GetCharArrayRegion = [](JNIEnv *env, jcharArray arr, jsize i1, jsize i2, jchar *c) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetCharArrayRegion\n");
#endif
   return ni->getCharArrayRegion(arr, i1, i2, c);
  };
  GetShortArrayRegion = [](JNIEnv *env, jshortArray arr, jsize i1, jsize i2, jshort *s) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetShortArrayRegion\n");
#endif
   return ni->getShortArrayRegion(arr, i1, i2, s);
  };
  GetIntArrayRegion = [](JNIEnv *env, jintArray arr, jsize i1, jsize i2, jint *i) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetIntArrayRegion\n");
#endif
   return ni->getIntArrayRegion(arr, i1, i2, i);
  };
  GetLongArrayRegion = [](JNIEnv *env, jlongArray arr, jsize i1, jsize i2, jlong *l) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetLongArrayRegion\n");
#endif
   return ni->getLongArrayRegion(arr, i1, i2, l);
  };
  GetFloatArrayRegion = [](JNIEnv *env, jfloatArray arr, jsize i1, jsize i2, jfloat *f) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetFloatArrayRegion\n");
#endif
   return ni->getFloatArrayRegion(arr, i1, i2, f);
  };
  GetDoubleArrayRegion = [](JNIEnv *env, jdoubleArray arr, jsize i1, jsize i2, jdouble *d) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetDoubleArrayRegion\n");
#endif
   return ni->getDoubleArrayRegion(arr, i1, i2, d);
  };
  SetBooleanArrayRegion = [](JNIEnv *env, jbooleanArray arr, jsize i1, jsize i2, const jboolean *b) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetBooleanArrayRegion\n");
#endif
   return ni->setBooleanArrayRegion(arr, i1, i2, b);
  };
  SetByteArrayRegion = [](JNIEnv *env, jbyteArray arr, jsize i1, jsize i2, const jbyte *b) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetByteArrayRegion\n");
#endif
   return ni->setByteArrayRegion(arr, i1, i2, b);
  };
  SetCharArrayRegion = [](JNIEnv *env, jcharArray arr, jsize i1, jsize i2, const jchar *c) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetCharArrayRegion\n");
#endif
   return ni->setCharArrayRegion(arr, i1, i2, c);
  };
  SetShortArrayRegion = [](JNIEnv *env, jshortArray arr, jsize i1, jsize i2, const jshort *s) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetShortArrayRegion\n");
#endif
   return ni->setShortArrayRegion(arr, i1, i2, s);
  };
  SetIntArrayRegion = [](JNIEnv *env, jintArray arr, jsize i1, jsize i2, const jint *i) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetIntArrayRegion\n");
#endif
   return ni->setIntArrayRegion(arr, i1, i2, i);
  };
  SetLongArrayRegion = [](JNIEnv *env, jlongArray arr, jsize i1, jsize i2, const jlong *l) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetLongArrayRegion\n");
#endif
   return ni->setLongArrayRegion(arr, i1, i2, l);
  };
  SetFloatArrayRegion = [](JNIEnv *env, jfloatArray arr, jsize i1, jsize i2, const jfloat *f) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetFloatArrayRegion\n");
#endif
   return ni->setFloatArrayRegion(arr, i1, i2, f);
  };
  SetDoubleArrayRegion = [](JNIEnv *env, jdoubleArray arr, jsize i1, jsize i2, const jdouble *d) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::SetDoubleArrayRegion\n");
#endif
   return ni->setDoubleArrayRegion(arr, i1, i2, d);
  };
  RegisterNatives = [](JNIEnv *env, jclass clazz, const JNINativeMethod *m, jint i) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::RegisterNatives\n");
#endif
   return ni->registerNatives(clazz, m, i);
  };
  UnregisterNatives = [](JNIEnv *env, jclass clazz) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::UnregisterNatives\n");
#endif
   return ni->unregisterNatives(clazz);
  };
  MonitorEnter = [](JNIEnv *env, jobject obj) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::MonitorEnter\n");
#endif
   return ni->monitorEnter(obj);
  };
  MonitorExit = [](JNIEnv *env, jobject obj) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::MonitorExit\n");
#endif
   return ni->monitorExit(obj);
  };
  GetJavaVM = [](JNIEnv *env, JavaVM **pvm) -> jint {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetJavaVM\n");
#endif
   return ni->getJavaVM(pvm);
  };
  GetStringRegion = [](JNIEnv *env, jstring str1, jsize i1, jsize i2, jchar *str2) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStringRegion\n");
#endif
   return ni->getStringRegion(str1, i1, i2, str2);
  };
  GetStringUTFRegion = [](JNIEnv *env, jstring str1, jsize i1, jsize i2, char *str2) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStringUTFRegion\n");
#endif
   return ni->getStringUTFRegion(str1, i1, i2, str2);
  };
  GetPrimitiveArrayCritical = [](JNIEnv *env, jarray arr, jboolean *b) -> void * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetPrimitiveArrayCritical\n");
#endif
   return ni->getPrimitiveArrayCritical(arr, b);
  };
  ReleasePrimitiveArrayCritical = [](JNIEnv *env, jarray arr, void *carr, jint i) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ReleasePrimitiveArrayCritical\n");
#endif
   return ni->releasePrimitiveArrayCritical(arr, carr, i);
  };
  GetStringCritical = [](JNIEnv *env, jstring str, jboolean *b) -> const jchar * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetStringCritical\n");
#endif
   return ni->getStringCritical(str, b);
  };
  ReleaseStringCritical = [](JNIEnv *env, jstring str, const jchar *c) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ReleaseStringCritical\n");
#endif
   return ni->releaseStringCritical(str, c);
  };
  NewWeakGlobalRef = [](JNIEnv *env, jobject obj) -> jweak {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewWeakGlobalRef\n");
#endif
   return ni->newWeakGlobalRef(obj);
  };
  DeleteWeakGlobalRef = [](JNIEnv *env, jweak w) -> void {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::DeleteWeakGlobalRef\n");
#endif
   return ni->deleteWeakGlobalRef(w);
  };
  ExceptionCheck = [](JNIEnv *env) -> jboolean {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::ExceptionCheck\n");
#endif
   return ni->exceptionCheck();
  };
  NewDirectByteBuffer = [](JNIEnv *env, void *addr, jlong l) -> jobject {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::NewDirectByteBuffer\n");
#endif
   return ni->newDirectByteBuffer(addr, l);
  };
  GetDirectBufferAddress = [](JNIEnv *env, jobject obj) -> void * {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetDirectBufferAddress\n");
#endif
   return ni->getDirectBufferAddress(obj);
  };
  GetDirectBufferCapacity = [](JNIEnv *env, jobject obj) -> jlong {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetDirectBufferCapacity\n");
#endif
   return ni->getDirectBufferCapacity(obj);
  };
  GetObjectRefType = [](JNIEnv *env, jobject obj) -> jobjectRefType {
   _FETCH_ENV_
#ifdef FAKE_JNI_DEBUG
   fprintf(ni->vm->getLog(), "DEBUG: JNINativeInterface_::GetObjectRefType\n");
#endif
   return ni->getObjectRefType(obj);
  };
 }
}