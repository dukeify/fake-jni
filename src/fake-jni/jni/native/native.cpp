#include "jni.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
 NativeInterface::NativeInterface(Jvm * const vm) :
  JNINativeInterface_(),
  vm(vm)
 {
  reserved0
   = reserved1
   = reserved2
   = reserved3
   = nullptr;
  GetVersion = [](JNIEnv *env) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetVersion)
   return ni->getVersion();
  };
  DefineClass = [](JNIEnv *env, const char *str, jobject obj, const jbyte *bytes, jsize size) -> jclass {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(DefineClass)
   return ni->defineClass(str, obj, bytes, size);
  };
  FindClass = [](JNIEnv *env, const char *str) -> jclass {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(FindClass)
   return ni->findClass(str);
  };
  FromReflectedMethod = [](JNIEnv *env, jobject obj) -> jmethodID {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(FromReflectedMethod)
   return ni->fromReflectedMethod(obj);
  };
  FromReflectedField = [](JNIEnv *env, jobject obj) -> jfieldID {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(FromReflectedField)
   return ni->fromReflectedField(obj);
  };
  ToReflectedMethod = [](JNIEnv *env, jclass clazz, jmethodID jmid, jboolean b) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ToReflectedField)
   return ni->toReflectedMethod(clazz, jmid, b);
  };
  GetSuperclass = [](JNIEnv *env, jclass clazz) -> jclass {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetSuperclass)
   return ni->getSuperclass(clazz);
  };
  IsAssignableFrom = [](JNIEnv *env, jclass clazz1, jclass clazz2) -> jboolean {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(IsAssignableFrom)
   return ni->isAssignableFrom(clazz1, clazz2);
  };
  ToReflectedField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jboolean b) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ToReflectedField)
   return ni->toReflectedField(clazz, jfid, b);
  };
  Throw = [](JNIEnv *env, jthrowable t) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(Throw)
   return ni->throw_(t);
  };
  ThrowNew = [](JNIEnv *env, jclass clazz, const char *str) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ThrowNew)
   return ni->throwNew(clazz, str);
  };
  ExceptionOccurred = [](JNIEnv *env) -> jthrowable {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ExceptionOccurred)
   return ni->exceptionOccurred();
  };
  ExceptionDescribe = [](JNIEnv *env) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ExceptionDescribe)
   ni->exceptionDescribe();
  };
  ExceptionClear = [](JNIEnv *env) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ExceptionClear)
   ni->exceptionClear();
  };
  FatalError = [](JNIEnv *env, const char *str) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(FatalError)
   ni->fatalError(str);
  };
  PushLocalFrame = [](JNIEnv *env, jint i) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(PushLocalFrame)
   return ni->pushLocalFrame(i);
  };
  PopLocalFrame = [](JNIEnv *env, jobject obj) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(PopLocalFrame)
   return ni->popLocalFrame(obj);
  };
  NewGlobalRef = [](JNIEnv *env, jobject obj) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewGlobalRef)
   return ni->newGlobalRef(obj);
  };
  DeleteGlobalRef = [](JNIEnv *env, jobject obj) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(DeleteGlobalRef)
   ni->deleteGlobalRef(obj);
  };
  DeleteLocalRef = [](JNIEnv *env, jobject obj) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(DeleteLocalRef)
   ni->deleteLocalRef(obj);
  };
  IsSameObject = [](JNIEnv *env, jobject obj1, jobject obj2) -> jboolean {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(IsSameObject)
   return ni->isSameObject(obj1, obj2);
  };
  NewLocalRef = [](JNIEnv *env, jobject obj) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewLocalRef)
   return ni->newLocalRef(obj);
  };
  EnsureLocalCapacity = [](JNIEnv *env, jint i) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(EnsureLocalCapacity)
   return ni->ensureLocalCapacity(i);
  };
  AllocObject = [](JNIEnv *env, jclass clazz) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(AllocObject)
   return ni->allocObject(clazz);
  };
  NewObject = &NativeInterface::_NewObject;
  NewObjectV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewObject)
   return ni->newObjectV(clazz, jmid, l);
  };
  NewObjectA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *value) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewObjectA)
   return ni->newObjectA(clazz, jmid, value);
  };
  GetObjectClass = [](JNIEnv *env, jobject obj) -> jclass {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetObjectClass)
   return ni->getObjectClass(obj);
  };
  IsInstanceOf = [](JNIEnv *env, jobject obj, jclass clazz) -> jboolean {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(IsInstanceOf)
   return ni->isInstanceOf(obj, clazz);
  };
  GetMethodID = [](JNIEnv *env, jclass clazz, const char *str1, const char *str2) -> jmethodID {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetMethodID)
   return ni->getMethodID(clazz, str1, str2);
  };
  CallObjectMethod = &NativeInterface::_CallObjectMethod;
  CallObjectMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallObjectMethodV)
   return ni->callObjectMethodV(obj, jmid, l);
  };
  CallObjectMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallObjectMethodA)
   return ni->callObjectMethodA(obj, jmid, val);
  };
  CallBooleanMethod = &NativeInterface::_CallBooleanMethod;
  CallBooleanMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jboolean {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallBooleanMethodV)
   return ni->callBooleanMethodV(obj, jmid, l);
  };
  CallBooleanMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jboolean {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallBooleanMethodA)
   return ni->callBooleanMethodA(obj, jmid, val);
  };
  CallByteMethod = &NativeInterface::_CallByteMethod;
  CallByteMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jbyte {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallByteMethodV)
   return ni->callByteMethodV(obj, jmid, l);
  };
  CallByteMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jbyte {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallByteMethodA)
   return ni->callByteMethodA(obj, jmid, val);
  };
  CallCharMethod = &NativeInterface::_CallCharMethod;
  CallCharMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jchar {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallCharMethodV)
   return ni->callCharMethodV(obj, jmid, l);
  };
  CallCharMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jchar {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallCharMethodA)
   return ni->callCharMethodA(obj, jmid, val);
  };
  CallShortMethod = &NativeInterface::_CallShortMethod;
  CallShortMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jshort {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallShortMethodV)
   return ni->callShortMethodV(obj, jmid, l);
  };
  CallShortMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jshort {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallShortMethodA)
   return ni->callShortMethodA(obj, jmid, val);
  };
  CallIntMethod = &NativeInterface::_CallIntMethod;
  CallIntMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallIntMethodV)
   return ni->callIntMethodV(obj, jmid, l);
  };
  CallIntMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallIntMethodA)
   return ni->callIntMethodA(obj, jmid, val);
  };
  CallLongMethod = &NativeInterface::_CallLongMethod;
  CallLongMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jlong {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallLongMethodV)
   return ni->callLongMethodV(obj, jmid, l);
  };
  CallLongMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jlong {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallLongmethodA)
   return ni->callLongMethodA(obj, jmid, val);
  };
  CallFloatMethod = &NativeInterface::_CallFloatMethod;
  CallFloatMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jfloat {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallFloatMethodV)
   return ni->callFloatMethodV(obj, jmid, l);
  };
  CallFloatMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jfloat {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallFloatMethodA)
   return ni->callFloatMethodA(obj, jmid, val);
  };
  CallDoubleMethod = &NativeInterface::_CallDoubleMethod;
  CallDoubleMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> jdouble {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallDoubleMethodV)
   return ni->callDoubleMethodV(obj, jmid, l);
  };
  CallDoubleMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> jdouble {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallDoubleMethodA)
   return ni->callDoubleMethodA(obj, jmid, val);
  };
  CallVoidMethod = &NativeInterface::_CallVoidMethod;
  CallVoidMethodV = [](JNIEnv *env, jobject obj, jmethodID jmid, va_list l) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallVoidMethodV)
   return ni->callVoidMethodV(obj, jmid, l);
  };
  CallVoidMethodA = [](JNIEnv *env, jobject obj, jmethodID jmid, const jvalue *val) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallVoidMethodA)
   return ni->callVoidMethodA(obj, jmid, val);
  };
  CallNonvirtualObjectMethod = &NativeInterface::_CallNonvirtualObjectMethod;
  CallNonvirtualObjectMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualObjectMethodV)
   return ni->callNonvirtualObjectMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualObjectMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualObjectMethodA)
   return ni->callNonvirtualObjectMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualBooleanMethod = &NativeInterface::_CallNonvirtualBooleanMethod;
  CallNonvirtualBooleanMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jboolean {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualBooleanMethodV)
   return ni->callNonvirtualBooleanMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualBooleanMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> jboolean {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualBooleanMethodA)
   return ni->callNonvirtualBooleanMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualByteMethod = &NativeInterface::_CallNonvirtualByteMethod;
  CallNonvirtualByteMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jbyte {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualByteMethodV)
   return ni->callNonvirtualByteMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualByteMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> jbyte {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualByteMethodA)
   return ni->callNonvirtualByteMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualCharMethod = &NativeInterface::_CallNonvirtualCharMethod;
  CallNonvirtualCharMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jchar {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualCharMethodV)
   return ni->callNonvirtualCharMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualCharMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> jchar {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualCharMethodA)
   return ni->callNonvirtualCharMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualShortMethod = &NativeInterface::_CallNonvirtualShortMethod;
  CallNonvirtualShortMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jshort {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualShortMethodV)
   return ni->callNonvirtualShortMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualShortMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> jshort {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualShortMethodA)
   return ni->callNonvirtualShortMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualIntMethod = &NativeInterface::_CallNonvirtualIntMethod;
  CallNonvirtualIntMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualIntMethodV)
   return ni->callNonvirtualIntMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualIntMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualIntMethodA)
   return ni->callNonvirtualIntMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualLongMethod = &NativeInterface::_CallNonvirtualLongMethod;
  CallNonvirtualLongMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jlong {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualLongMethodV)
   return ni->callNonvirtualLongMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualLongMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> jlong {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualLongMethodA)
   return ni->callNonvirtualLongMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualFloatMethod = &NativeInterface::_CallNonvirtualFloatMethod;
  CallNonvirtualFloatMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jfloat {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualFloatMethodV)
   return ni->callNonvirtualFloatMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualFloatMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid,
                                  const jvalue *val) -> jfloat {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualFloatMethodA)
   return ni->callNonvirtualFloatMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualDoubleMethod = &NativeInterface::_CallNonvirtualDoubleMethod;
  CallNonvirtualDoubleMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> jdouble {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualDoubleMethodV)
   return ni->callNonvirtualDoubleMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualDoubleMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid,
                                   const jvalue *val) -> jdouble {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualDoubleMethodA)
   return ni->callNonvirtualDoubleMethodA(obj, clazz, jmid, val);
  };
  CallNonvirtualVoidMethod = &NativeInterface::_CallNonvirtualVoidMethod;
  CallNonvirtualVoidMethodV = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, va_list l) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualVoidMethodV)
   return ni->callNonvirtualVoidMethodV(obj, clazz, jmid, l);
  };
  CallNonvirtualVoidMethodA = [](JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, const jvalue *val) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallNonvirtualVoidMethodA)
   return ni->callNonvirtualVoidMethodA(obj, clazz, jmid, val);
  };
  GetFieldID = [](JNIEnv *env, jclass clazz, const char *str1, const char *str2) -> jfieldID {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetFieldID)
   return ni->getFieldID(clazz, str1, str2);
  };
  GetObjectField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetObjectField)
   return ni->getObjectField(obj, jfid);
  };
  GetBooleanField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jboolean {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetBooleanField)
   return ni->getBooleanField(obj, jfid);
  };
  GetByteField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jbyte {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetByteField)
   return ni->getByteField(obj, jfid);
  };
  GetCharField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jchar {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetCharField)
   return ni->getCharField(obj, jfid);
  };
  GetShortField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jshort {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetShortField)
   return ni->getShortField(obj, jfid);
  };
  GetIntField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetIntField)
   return ni->getIntField(obj, jfid);
  };
  GetLongField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jlong {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetLongField)
   return ni->getLongField(obj, jfid);
  };
  GetFloatField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jfloat {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetFloatField)
   return ni->getFloatField(obj, jfid);
  };
  GetDoubleField = [](JNIEnv *env, jobject obj, jfieldID jfid) -> jdouble {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetDoubleField)
   return ni->getDoubleField(obj, jfid);
  };
  SetObjectField = [](JNIEnv *env, jobject obj1, jfieldID jfid, jobject obj2) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetObjectField)
   return ni->setObjectField(obj1, jfid, obj2);
  };
  SetBooleanField = [](JNIEnv *env, jobject obj, jfieldID jfid, jboolean b) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetBooleanField)
   return ni->setBooleanField(obj, jfid, b);
  };
  SetByteField = [](JNIEnv *env, jobject obj, jfieldID jfid, jbyte b) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetByteField)
   return ni->setByteField(obj, jfid, b);
  };
  SetCharField = [](JNIEnv *env, jobject obj, jfieldID jfid, jchar c) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetCharField)
   return ni->setCharField(obj, jfid, c);
  };
  SetShortField = [](JNIEnv *env, jobject obj, jfieldID jfid, jshort s) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetShortField)
   return ni->setShortField(obj, jfid, s);
  };
  SetIntField = [](JNIEnv *env, jobject obj, jfieldID jfid, jint i) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetIntField)
   return ni->setIntField(obj, jfid, i);
  };
  SetLongField = [](JNIEnv *env, jobject obj, jfieldID jfid, jlong l) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetLongField)
   return ni->setLongField(obj, jfid, l);
  };
  SetFloatField = [](JNIEnv *env, jobject obj, jfieldID jfid, jfloat f) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetFloatField)
   return ni->setFloatField(obj, jfid, f);
  };
  SetDoubleField = [](JNIEnv *env, jobject obj, jfieldID jfid, jdouble d) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetDoubleField)
   return ni->setDoubleField(obj, jfid, d);
  };
  GetStaticMethodID = [](JNIEnv *env, jclass clazz, const char *str1, const char *str2) -> jmethodID {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStaticMethodID)
   return ni->getStaticMethodID(clazz, str1, str2);
  };
  CallStaticObjectMethod = &NativeInterface::_CallStaticObjectMethod;
  CallStaticObjectMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticObjectMethodV)
   return ni->callStaticObjectMethodV(clazz, jmid, l);
  };
  CallStaticObjectMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticObjectMethodA)
   return ni->callStaticObjectMethodA(clazz, jmid, val);
  };
  CallStaticBooleanMethod = &NativeInterface::_CallStaticBooleanMethod;
  CallStaticBooleanMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jboolean {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticBooleanMethodV)
   return ni->callStaticBooleanMethodV(clazz, jmid, l);
  };
  CallStaticBooleanMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jboolean {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticBooleanMethodA)
   return ni->callStaticBooleanMethodA(clazz, jmid, val);
  };
  CallStaticByteMethod = &NativeInterface::_CallStaticByteMethod;
  CallStaticByteMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jbyte {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticByteMethodV)
   return ni->callStaticByteMethodV(clazz, jmid, l);
  };
  CallStaticByteMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jbyte {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticByteMethodA)
   return ni->callStaticByteMethodA(clazz, jmid, val);
  };
  CallStaticCharMethod = &NativeInterface::_CallStaticCharMethod;
  CallStaticCharMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jchar {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticCharMethodV)
   return ni->callStaticCharMethodV(clazz, jmid, l);
  };
  CallStaticCharMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jchar {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticCharMethodA)
   return ni->callStaticCharMethodA(clazz, jmid, val);
  };
  CallStaticShortMethod = &NativeInterface::_CallStaticShortMethod;
  CallStaticShortMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jshort {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticShortMethodV)
   return ni->callStaticShortMethodV(clazz, jmid, l);
  };
  CallStaticShortMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jshort {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticShortMethodA)
   return ni->callStaticShortMethodA(clazz, jmid, val);
  };
  CallStaticIntMethod = &NativeInterface::_CallStaticIntMethod;
  CallStaticIntMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticIntMethodV)
   return ni->callStaticIntMethodV(clazz, jmid, l);
  };
  CallStaticIntMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticIntMethodA)
   return ni->callStaticIntMethodA(clazz, jmid, val);
  };
  CallStaticLongMethod = &NativeInterface::_CallStaticLongMethod;
  CallStaticLongMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jlong {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticLongMethodV)
   return ni->callStaticLongMethodV(clazz, jmid, l);
  };
  CallStaticLongMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jlong {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticLongMethodA)
   return ni->callStaticLongMethodA(clazz, jmid, val);
  };
  CallStaticFloatMethod = &NativeInterface::_CallStaticFloatMethod;
  CallStaticFloatMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jfloat {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticFloatMethodV)
   return ni->callStaticFloatMethodV(clazz, jmid, l);
  };
  CallStaticFloatMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jfloat {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticFloatMethodA)
   return ni->callStaticFloatMethodA(clazz, jmid, val);
  };
  CallStaticDoubleMethod = &NativeInterface::_CallStaticDoubleMethod;
  CallStaticDoubleMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> jdouble {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticDoubleMethodV)
   return ni->callStaticDoubleMethodV(clazz, jmid, l);
  };
  CallStaticDoubleMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> jdouble {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticDoubleMethodA)
   return ni->callStaticDoubleMethodA(clazz, jmid, val);
  };
  CallStaticVoidMethod = &NativeInterface::_CallStaticVoidMethod;
  CallStaticVoidMethodV = [](JNIEnv *env, jclass clazz, jmethodID jmid, va_list l) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticVoidMethodV)
   return ni->callStaticVoidMethodV(clazz, jmid, l);
  };
  CallStaticVoidMethodA = [](JNIEnv *env, jclass clazz, jmethodID jmid, const jvalue *val) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(CallStaticVoidMethodA)
   return ni->callStaticVoidMethodA(clazz, jmid, val);
  };
  GetStaticFieldID = [](JNIEnv *env, jclass clazz, const char *str1, const char *str2) -> jfieldID {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStaticFieldID)
   return ni->getStaticFieldID(clazz, str1, str2);
  };
  GetStaticObjectField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStaticObjectField)
   return ni->getStaticObjectField(clazz, jfid);
  };
  GetStaticBooleanField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jboolean {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStaticBooleanField)
   return ni->getStaticBooleanField(clazz, jfid);
  };
  GetStaticByteField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jbyte {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStaticByteField)
   return ni->getStaticByteField(clazz, jfid);
  };
  GetStaticCharField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jchar {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStaticCharField)
   return ni->getStaticCharField(clazz, jfid);
  };
  GetStaticShortField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jshort {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStaticShortField)
   return ni->getStaticShortField(clazz, jfid);
  };
  GetStaticIntField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStaticIntField)
   return ni->getStaticIntField(clazz, jfid);
  };
  GetStaticLongField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jlong {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStaticLongField)
   return ni->getStaticLongField(clazz, jfid);
  };
  GetStaticFloatField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jfloat {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStaticFloatField)
   return ni->getStaticFloatField(clazz, jfid);
  };
  GetStaticDoubleField = [](JNIEnv *env, jclass clazz, jfieldID jfid) -> jdouble {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStaticDoubleField)
   return ni->getStaticDoubleField(clazz, jfid);
  };
  SetStaticObjectField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jobject obj) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetStaticObjectField)
   return ni->setStaticObjectField(clazz, jfid, obj);
  };
  SetStaticBooleanField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jboolean b) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetStaticBooleanField)
   return ni->setStaticBooleanField(clazz, jfid, b);
  };
  SetStaticByteField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jbyte b) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetStaticByteField)
   return ni->setStaticByteField(clazz, jfid, b);
  };
  SetStaticCharField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jchar c) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetStaticCharField)
   return ni->setStaticCharField(clazz, jfid, c);
  };
  SetStaticShortField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jshort s) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetStaticShortField)
   return ni->setStaticShortField(clazz, jfid, s);
  };
  SetStaticIntField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jint i) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetStaticIntField)
   return ni->setStaticIntField(clazz, jfid, i);
  };
  SetStaticLongField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jlong l) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetStaticLongField)
   return ni->setStaticLongField(clazz, jfid, l);
  };
  SetStaticFloatField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jfloat f) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetStaticFloatField)
   return ni->setStaticFloatField(clazz, jfid, f);
  };
  SetStaticDoubleField = [](JNIEnv *env, jclass clazz, jfieldID jfid, jdouble d) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetStaticDoubleField)
   return ni->setStaticDoubleField(clazz, jfid, d);
  };
  NewString = [](JNIEnv *env, const jchar *str, jsize size) -> jstring {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewString)
   return ni->newString(str, size);
  };
  GetStringLength = [](JNIEnv *env, jstring str) -> jsize {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStringLength)
   return ni->getStringLength(str);
  };
  GetStringChars = [](JNIEnv *env, jstring str, jboolean *b) -> const jchar * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStringChars)
   return ni->getStringChars(str, b);
  };
  ReleaseStringChars = [](JNIEnv *env, jstring str1, const jchar *str2) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ReleaseStringChars)
   return ni->releaseStringChars(str1, str2);
  };
  NewStringUTF = [](JNIEnv *env, const char *str) -> jstring {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewStringUTF)
   return ni->newStringUTF(str);
  };
  GetStringUTFLength = [](JNIEnv *env, jstring str) -> jsize {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStringUTFLength)
   return ni->getStringUTFLength(str);
  };
  GetStringUTFChars = [](JNIEnv *env, jstring str, jboolean *b) -> const char * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStringUTFChars)
   return ni->getStringUTFChars(str, b);
  };
  ReleaseStringUTFChars = [](JNIEnv *env, jstring str1, const char *str2) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ReleaseStringUTFChars)
   return ni->releaseStringUTFChars(str1, str2);
  };
  GetArrayLength = [](JNIEnv *env, jarray arr) -> jsize {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetArrayLength)
   return ni->getArrayLength(arr);
  };
  NewObjectArray = [](JNIEnv *env, jsize size, jclass clazz, jobject obj) -> jobjectArray {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewObjectArray)
   return ni->newObjectArray(size, clazz, obj);
  };
  GetObjectArrayElement = [](JNIEnv *env, jobjectArray arr, jsize size) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetObjectArrayElement)
   return ni->getObjectArrayElement(arr, size);
  };
  SetObjectArrayElement = [](JNIEnv *env, jobjectArray arr, jsize size, jobject obj) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetObjectArrayElement)
   return ni->setObjectArrayElement(arr, size, obj);
  };
  NewBooleanArray = [](JNIEnv *env, jsize size) -> jbooleanArray {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewBooleanArray)
   return ni->newBooleanArray(size);
  };
  NewByteArray = [](JNIEnv *env, jsize size) -> jbyteArray {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewByteArray)
   return ni->newByteArray(size);
  };
  NewCharArray = [](JNIEnv *env, jsize size) -> jcharArray {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewCharArray)
   return ni->newCharArray(size);
  };
  NewShortArray = [](JNIEnv *env, jsize size) -> jshortArray {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewShortArray)
   return ni->newShortArray(size);
  };
  NewIntArray = [](JNIEnv *env, jsize size) -> jintArray {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewIntArray)
   return ni->newIntArray(size);
  };
  NewLongArray = [](JNIEnv *env, jsize size) -> jlongArray {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewLongArray)
   return ni->newLongArray(size);
  };
  NewFloatArray = [](JNIEnv *env, jsize size) -> jfloatArray {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewFloatArray)
   return ni->newFloatArray(size);
  };
  NewDoubleArray = [](JNIEnv *env, jsize size) -> jdoubleArray {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewDoubleArray)
   return ni->newDoubleArray(size);
  };
  GetBooleanArrayElements = [](JNIEnv *env, jbooleanArray arr, jboolean *b) -> jboolean * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetBooleanArrayElements)
   return ni->getBooleanArrayElements(arr, b);
  };
  GetByteArrayElements = [](JNIEnv *env, jbyteArray arr, jboolean *b) -> jbyte * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetByteArrayElements)
   return ni->getByteArrayElements(arr, b);
  };
  GetCharArrayElements = [](JNIEnv *env, jcharArray arr, jboolean *b) -> jchar * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetCharArrayElements)
   return ni->getCharArrayElements(arr, b);
  };
  GetShortArrayElements = [](JNIEnv *env, jshortArray arr, jboolean *b) -> jshort * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetShortArrayElements)
   return ni->getShortArrayElements(arr, b);
  };
  GetIntArrayElements = [](JNIEnv *env, jintArray arr, jboolean *b) -> jint * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetIntArrayElements)
   return ni->getIntArrayElements(arr, b);
  };
  GetLongArrayElements = [](JNIEnv *env, jlongArray arr, jboolean *b) -> jlong * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetLongArrayElements)
   return ni->getLongArrayElements(arr, b);
  };
  GetFloatArrayElements = [](JNIEnv *env, jfloatArray arr, jboolean *b) -> jfloat * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetFloatArrayElements)
   return ni->getFloatArrayElements(arr, b);
  };
  GetDoubleArrayElements = [](JNIEnv *env, jdoubleArray arr, jboolean *b) -> jdouble * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetDoubleArrayElements)
   return ni->getDoubleArrayElements(arr, b);
  };
  ReleaseBooleanArrayElements = [](JNIEnv *env, jbooleanArray arr, jboolean *b, jint i) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ReleaseBooleanArrayElements)
   return ni->releaseBooleanArrayElements(arr, b, i);
  };
  ReleaseByteArrayElements = [](JNIEnv *env, jbyteArray arr, jbyte *b, jint i) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ReleaseByteArrayElements)
   return ni->releaseByteArrayElements(arr, b, i);
  };
  ReleaseCharArrayElements = [](JNIEnv *env, jcharArray arr, jchar *c, jint i) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ReleaseCharArrayElements)
   return ni->releaseCharArrayElements(arr, c, i);
  };
  ReleaseShortArrayElements = [](JNIEnv *env, jshortArray arr, jshort *s, jint i) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ReleaseShortArrayElements)
   return ni->releaseShortArrayElements(arr, s, i);
  };
  ReleaseIntArrayElements = [](JNIEnv *env, jintArray arr, jint *i1, jint i2) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ReleaseIntArrayElements)
   return ni->releaseIntArrayElements(arr, i1, i2);
  };
  ReleaseLongArrayElements = [](JNIEnv *env, jlongArray arr, jlong *l, jint i) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ReleaseLongArrayElements)
   return ni->releaseLongArrayElements(arr, l, i);
  };
  ReleaseFloatArrayElements = [](JNIEnv *env, jfloatArray arr, jfloat *f, jint i) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ReleaseFloatArrayElements)
   return ni->releaseFloatArrayElements(arr, f, i);
  };
  ReleaseDoubleArrayElements = [](JNIEnv *env, jdoubleArray arr, jdouble *d, jint i) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ReleaseDoubleArrayElements)
   return ni->releaseDoubleArrayElements(arr, d, i);
  };
  GetBooleanArrayRegion = [](JNIEnv *env, jbooleanArray arr, jsize i1, jsize i2, jboolean *b) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetBooleanArrayRegion)
   return ni->getBooleanArrayRegion(arr, i1, i2, b);
  };
  GetByteArrayRegion = [](JNIEnv *env, jbyteArray arr, jsize i1, jsize i2, jbyte *b) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetByteArrayRegion)
   return ni->getByteArrayRegion(arr, i1, i2, b);
  };
  GetCharArrayRegion = [](JNIEnv *env, jcharArray arr, jsize i1, jsize i2, jchar *c) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetCharArrayRegion)
   return ni->getCharArrayRegion(arr, i1, i2, c);
  };
  GetShortArrayRegion = [](JNIEnv *env, jshortArray arr, jsize i1, jsize i2, jshort *s) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetShortArrayRegion)
   return ni->getShortArrayRegion(arr, i1, i2, s);
  };
  GetIntArrayRegion = [](JNIEnv *env, jintArray arr, jsize i1, jsize i2, jint *i) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetIntArrayRegion)
   return ni->getIntArrayRegion(arr, i1, i2, i);
  };
  GetLongArrayRegion = [](JNIEnv *env, jlongArray arr, jsize i1, jsize i2, jlong *l) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetLongArrayRegion)
   return ni->getLongArrayRegion(arr, i1, i2, l);
  };
  GetFloatArrayRegion = [](JNIEnv *env, jfloatArray arr, jsize i1, jsize i2, jfloat *f) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetFloatArrayRegion)
   return ni->getFloatArrayRegion(arr, i1, i2, f);
  };
  GetDoubleArrayRegion = [](JNIEnv *env, jdoubleArray arr, jsize i1, jsize i2, jdouble *d) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetDoubleArrayRegion)
   return ni->getDoubleArrayRegion(arr, i1, i2, d);
  };
  SetBooleanArrayRegion = [](JNIEnv *env, jbooleanArray arr, jsize i1, jsize i2, const jboolean *b) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetBooleanArrayRegion)
   return ni->setBooleanArrayRegion(arr, i1, i2, b);
  };
  SetByteArrayRegion = [](JNIEnv *env, jbyteArray arr, jsize i1, jsize i2, const jbyte *b) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetByteArrayRegion)
   return ni->setByteArrayRegion(arr, i1, i2, b);
  };
  SetCharArrayRegion = [](JNIEnv *env, jcharArray arr, jsize i1, jsize i2, const jchar *c) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetCharArrayRegion)
   return ni->setCharArrayRegion(arr, i1, i2, c);
  };
  SetShortArrayRegion = [](JNIEnv *env, jshortArray arr, jsize i1, jsize i2, const jshort *s) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetShortArrayRegion)
   return ni->setShortArrayRegion(arr, i1, i2, s);
  };
  SetIntArrayRegion = [](JNIEnv *env, jintArray arr, jsize i1, jsize i2, const jint *i) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetIntArrayRegion)
   return ni->setIntArrayRegion(arr, i1, i2, i);
  };
  SetLongArrayRegion = [](JNIEnv *env, jlongArray arr, jsize i1, jsize i2, const jlong *l) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetLongArrayRegion)
   return ni->setLongArrayRegion(arr, i1, i2, l);
  };
  SetFloatArrayRegion = [](JNIEnv *env, jfloatArray arr, jsize i1, jsize i2, const jfloat *f) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetFloatArrayRegion)
   return ni->setFloatArrayRegion(arr, i1, i2, f);
  };
  SetDoubleArrayRegion = [](JNIEnv *env, jdoubleArray arr, jsize i1, jsize i2, const jdouble *d) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(SetDoubleArrayRegion)
   return ni->setDoubleArrayRegion(arr, i1, i2, d);
  };
  RegisterNatives = [](JNIEnv *env, jclass clazz, const JNINativeMethod *m, jint i) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(RegisterNatives)
   return ni->registerNatives(clazz, m, i);
  };
  UnregisterNatives = [](JNIEnv *env, jclass clazz) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(UnregisterNatives)
   return ni->unregisterNatives(clazz);
  };
  MonitorEnter = [](JNIEnv *env, jobject obj) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(MonitorEnter)
   return ni->monitorEnter(obj);
  };
  MonitorExit = [](JNIEnv *env, jobject obj) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(MonitorExit)
   return ni->monitorExit(obj);
  };
  GetJavaVM = [](JNIEnv *env, JavaVM **pvm) -> jint {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetJavaVM)
   return ni->getJavaVM(pvm);
  };
  GetStringRegion = [](JNIEnv *env, jstring str1, jsize i1, jsize i2, jchar *str2) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStringRegion)
   return ni->getStringRegion(str1, i1, i2, str2);
  };
  GetStringUTFRegion = [](JNIEnv *env, jstring str1, jsize i1, jsize i2, char *str2) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStringUTFRegion)
   return ni->getStringUTFRegion(str1, i1, i2, str2);
  };
  GetPrimitiveArrayCritical = [](JNIEnv *env, jarray arr, jboolean *b) -> void * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetPrimitiveArrayCritical)
   return ni->getPrimitiveArrayCritical(arr, b);
  };
  ReleasePrimitiveArrayCritical = [](JNIEnv *env, jarray arr, void *carr, jint i) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ReleasePrimitiveArrayCritical)
   return ni->releasePrimitiveArrayCritical(arr, carr, i);
  };
  GetStringCritical = [](JNIEnv *env, jstring str, jboolean *b) -> const jchar * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetStringCritical)
   return ni->getStringCritical(str, b);
  };
  ReleaseStringCritical = [](JNIEnv *env, jstring str, const jchar *c) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ReleaseStringCritical)
   return ni->releaseStringCritical(str, c);
  };
  NewWeakGlobalRef = [](JNIEnv *env, jobject obj) -> jweak {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewWeakGlobalRef)
   return ni->newWeakGlobalRef(obj);
  };
  DeleteWeakGlobalRef = [](JNIEnv *env, jweak w) -> void {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(DeleteWeakGlobalRef)
   return ni->deleteWeakGlobalRef(w);
  };
  ExceptionCheck = [](JNIEnv *env) -> jboolean {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(ExceptionCheck)
   return ni->exceptionCheck();
  };
  NewDirectByteBuffer = [](JNIEnv *env, void *addr, jlong l) -> jobject {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(NewDirectByteBuffer)
   return ni->newDirectByteBuffer(addr, l);
  };
  GetDirectBufferAddress = [](JNIEnv *env, jobject obj) -> void * {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetDirectBufferAddress)
   return ni->getDirectBufferAddress(obj);
  };
  GetDirectBufferCapacity = [](JNIEnv *env, jobject obj) -> jlong {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetDirectBufferCapacity)
   return ni->getDirectBufferCapacity(obj);
  };
  GetObjectRefType = [](JNIEnv *env, jobject obj) -> jobjectRefType {
   _FETCH_JNI_ENV
   _NATIVE_DEBUG(GetObjectRefType)
   return ni->getObjectRefType(obj);
  };
 }
}