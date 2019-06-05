#pragma once

#include "fake_jni/types.h"

#ifdef FAKE_JNI_DEBUG
#include <cstdio>
#endif

#include <map>
#include <mutex>

namespace FakeJVM {
 class NativeInterface : public JNINativeInterface_ {
 private:
  //native_inlined.
  inline static jobject _NewObject(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  inline static jobject _CallObjectMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  inline static jboolean _CallBooleanMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  inline static jbyte _CallByteMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  inline static jchar _CallCharMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  inline static jshort _CallShortMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  inline static jint _CallIntMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  inline static jlong _CallLongMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  inline static jfloat _CallFloatMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  inline static jdouble _CallDoubleMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  inline static void _CallVoidMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  inline static jobject _CallNonvirtualObjectMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  inline static jboolean _CallNonvirtualBooleanMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  inline static jbyte _CallNonvirtualByteMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  inline static jchar _CallNonvirtualCharMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  inline static jshort _CallNonvirtualShortMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  inline static jint _CallNonvirtualIntMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  inline static jlong _CallNonvirtualLongMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  inline static jfloat _CallNonvirtualFloatMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  inline static jdouble _CallNonvirtualDoubleMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  inline static void _CallNonvirtualVoidMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  inline static jobject _CallStaticObjectMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  inline static jboolean _CallStaticBooleanMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  inline static jbyte _CallStaticByteMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  inline static jchar _CallStaticCharMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  inline static jshort _CallStaticShortMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  inline static jint _CallStaticIntMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  inline static jlong _CallStaticLongMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  inline static jfloat _CallStaticFloatMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  inline static jdouble _CallStaticDoubleMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  inline static void _CallStaticVoidMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);

 public:
  Jvm *const vm;

  //native_constructor.h
  explicit NativeInterface(Jvm * const vm);

  //TODO deallocate all JVM allocated memory
  ~NativeInterface() {}

  //TODO make all functions pure virtual

  //exception.h
  virtual jint throw_(jthrowable) const;
  virtual jint throwNew(jclass, const char *) const;
  virtual jthrowable exceptionOccurred() const;
  virtual void exceptionDescribe() const;
  virtual void exceptionClear() const;
  virtual jboolean exceptionCheck() const;
  virtual void fatalError(const char *) const;
  virtual jint pushLocalFrame(jint) const;
  virtual jobject popLocalFrame(jobject) const;

  //object.h
  virtual jboolean isSameObject(jobject, jobject) const;
  virtual jobject allocObject(jclass) const;
  virtual jobject newObject(jclass, jmethodID, ...) const;
  virtual jobject newObjectV(jclass, jmethodID, va_list) const;
  virtual jobject newObjectA(jclass, jmethodID, const jvalue *) const;
  virtual jclass getObjectClass(jobject) const;
  virtual jboolean isInstanceOf(jobject, jclass) const;
  virtual jclass defineClass(const char *, jobject, const jbyte *, jsize) const;
  virtual jclass findClass(const char *) const;
  virtual jclass getSuperclass(jclass) const;
  virtual jboolean isAssignableFrom(jclass, jclass) const;

  //misc.h
  virtual jint ensureLocalCapacity(jint) const;
  virtual jint getVersion() const;
  virtual jint registerNatives(jclass, const JNINativeMethod *, jint) const;
  virtual jint unregisterNatives(jclass) const;
  virtual jint monitorEnter(jobject) const;
  virtual jint monitorExit(jobject) const;
  virtual jint getJavaVM(JavaVM **) const;

  //reflect.h
  virtual jmethodID fromReflectedMethod(jobject) const;
  virtual jfieldID fromReflectedField(jobject) const;
  virtual jobject toReflectedMethod(jclass, jmethodID, jboolean) const;
  virtual jobject toReflectedField(jclass, jfieldID, jboolean) const;

  //method.h
  virtual jmethodID getMethodID(jclass, const char *, const char *) const;
  virtual jobject callObjectMethod(jobject, jmethodID, ...) const;
  virtual jobject callObjectMethodV(jobject, jmethodID, va_list) const;
  virtual jobject callObjectMethodA(jobject, jmethodID, const jvalue *) const;
  virtual jboolean callBooleanMethod(jobject, jmethodID, ...) const;
  virtual jboolean callBooleanMethodV(jobject, jmethodID, va_list) const;
  virtual jboolean callBooleanMethodA(jobject, jmethodID, const jvalue *) const;
  virtual jbyte callByteMethod(jobject, jmethodID, ...) const;
  virtual jbyte callByteMethodV(jobject, jmethodID, va_list) const;
  virtual jbyte callByteMethodA(jobject, jmethodID, const jvalue *) const;
  virtual jchar callCharMethod(jobject, jmethodID, ...) const;
  virtual jchar callCharMethodV(jobject, jmethodID, va_list) const;
  virtual jchar callCharMethodA(jobject, jmethodID, const jvalue *) const;
  virtual jshort callShortMethod(jobject, jmethodID, ...) const;
  virtual jshort callShortMethodV(jobject, jmethodID, va_list) const;
  virtual jshort callShortMethodA(jobject, jmethodID, const jvalue *) const;
  virtual jint callIntMethod(jobject, jmethodID, ...) const;
  virtual jint callIntMethodV(jobject, jmethodID, va_list) const;
  virtual jint callIntMethodA(jobject, jmethodID, const jvalue *) const;
  virtual jlong callLongMethod(jobject, jmethodID, ...) const;
  virtual jlong callLongMethodV(jobject, jmethodID, va_list) const;
  virtual jlong callLongMethodA(jobject, jmethodID, const jvalue *) const;
  virtual jfloat callFloatMethod(jobject, jmethodID, ...) const;
  virtual jfloat callFloatMethodV(jobject, jmethodID, va_list) const;
  virtual jfloat callFloatMethodA(jobject, jmethodID, const jvalue *) const;
  virtual jdouble callDoubleMethod(jobject, jmethodID, ...) const;
  virtual jdouble callDoubleMethodV(jobject, jmethodID, va_list) const;
  virtual jdouble callDoubleMethodA(jobject, jmethodID, const jvalue *) const;
  virtual void callVoidMethod(jobject, jmethodID, ...) const;
  virtual void callVoidMethodV(jobject, jmethodID, va_list) const;
  virtual void callVoidMethodA(jobject, jmethodID, const jvalue *) const;
  virtual jobject callNonvirtualObjectMethod(jobject, jclass, jmethodID, ...) const;
  virtual jobject callNonvirtualObjectMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jobject callNonvirtualObjectMethodA(jobject, jclass, jmethodID, const jvalue *) const;
  virtual jboolean callNonvirtualBooleanMethod(jobject, jclass, jmethodID, ...) const;
  virtual jboolean callNonvirtualBooleanMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jboolean callNonvirtualBooleanMethodA(jobject, jclass, jmethodID, const jvalue *) const;
  virtual jbyte callNonvirtualByteMethod(jobject, jclass, jmethodID, ...) const;
  virtual jbyte callNonvirtualByteMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jbyte callNonvirtualByteMethodA(jobject, jclass, jmethodID, const jvalue *) const;
  virtual jchar callNonvirtualCharMethod(jobject, jclass, jmethodID, ...) const;
  virtual jchar callNonvirtualCharMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jchar callNonvirtualCharMethodA(jobject, jclass, jmethodID, const jvalue *) const;
  virtual jshort callNonvirtualShortMethod(jobject, jclass, jmethodID, ...) const;
  virtual jshort callNonvirtualShortMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jshort callNonvirtualShortMethodA(jobject, jclass, jmethodID, const jvalue *) const;
  virtual jint callNonvirtualIntMethod(jobject, jclass, jmethodID, ...) const;
  virtual jint callNonvirtualIntMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jint callNonvirtualIntMethodA(jobject, jclass, jmethodID, const jvalue *) const;
  virtual jlong callNonvirtualLongMethod(jobject, jclass, jmethodID, ...) const;
  virtual jlong callNonvirtualLongMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jlong callNonvirtualLongMethodA(jobject, jclass, jmethodID, const jvalue *) const;
  virtual jfloat callNonvirtualFloatMethod(jobject, jclass, jmethodID, ...) const;
  virtual jfloat callNonvirtualFloatMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jfloat callNonvirtualFloatMethodA(jobject, jclass, jmethodID, const jvalue *) const;
  virtual jdouble callNonvirtualDoubleMethod(jobject, jclass, jmethodID, ...) const;
  virtual jdouble callNonvirtualDoubleMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jdouble callNonvirtualDoubleMethodA(jobject, jclass, jmethodID, const jvalue *) const;
  virtual void callNonvirtualVoidMethod(jobject, jclass, jmethodID, ...) const;
  virtual void callNonvirtualVoidMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual void callNonvirtualVoidMethodA(jobject, jclass, jmethodID, const jvalue *) const;
  virtual jmethodID getStaticMethodID(jclass, const char *, const char *) const;
  virtual jobject callStaticObjectMethod(jclass, jmethodID, ...) const;
  virtual jobject callStaticObjectMethodV(jclass, jmethodID, va_list) const;
  virtual jobject callStaticObjectMethodA(jclass, jmethodID, const jvalue *) const;
  virtual jboolean callStaticBooleanMethod(jclass, jmethodID, ...) const;
  virtual jboolean callStaticBooleanMethodV(jclass, jmethodID, va_list) const;
  virtual jboolean callStaticBooleanMethodA(jclass, jmethodID, const jvalue *) const;
  virtual jbyte callStaticByteMethod(jclass, jmethodID, ...) const;
  virtual jbyte callStaticByteMethodV(jclass, jmethodID, va_list) const;
  virtual jbyte callStaticByteMethodA(jclass, jmethodID, const jvalue *) const;
  virtual jchar callStaticCharMethod(jclass, jmethodID, ...) const;
  virtual jchar callStaticCharMethodV(jclass, jmethodID, va_list) const;
  virtual jchar callStaticCharMethodA(jclass, jmethodID, const jvalue *) const;
  virtual jshort callStaticShortMethod(jclass, jmethodID, ...) const;
  virtual jshort callStaticShortMethodV(jclass, jmethodID, va_list) const;
  virtual jshort callStaticShortMethodA(jclass, jmethodID, const jvalue *) const;
  virtual jint callStaticIntMethod(jclass, jmethodID, ...) const;
  virtual jint callStaticIntMethodV(jclass, jmethodID, va_list) const;
  virtual jint callStaticIntMethodA(jclass, jmethodID, const jvalue *) const;
  virtual jlong callStaticLongMethod(jclass, jmethodID, ...) const;
  virtual jlong callStaticLongMethodV(jclass, jmethodID, va_list) const;
  virtual jlong callStaticLongMethodA(jclass, jmethodID, const jvalue *) const;
  virtual jfloat callStaticFloatMethod(jclass, jmethodID, ...) const;
  virtual jfloat callStaticFloatMethodV(jclass, jmethodID, va_list) const;
  virtual jfloat callStaticFloatMethodA(jclass, jmethodID, const jvalue *) const;
  virtual jdouble callStaticDoubleMethod(jclass, jmethodID, ...) const;
  virtual jdouble callStaticDoubleMethodV(jclass, jmethodID, va_list) const;
  virtual jdouble callStaticDoubleMethodA(jclass, jmethodID, const jvalue *) const;
  virtual void callStaticVoidMethod(jclass, jmethodID, ...) const;
  virtual void callStaticVoidMethodV(jclass, jmethodID, va_list) const;
  virtual void callStaticVoidMethodA(jclass, jmethodID, const jvalue *) const;

  //field.h
  virtual jfieldID getFieldID(jclass, const char *, const char *) const;
  virtual jobject getObjectField(jobject, jfieldID) const;
  virtual jboolean getBooleanField(jobject, jfieldID) const;
  virtual jbyte getByteField(jobject, jfieldID) const;
  virtual jchar getCharField(jobject, jfieldID) const;
  virtual jshort getShortField(jobject, jfieldID) const;
  virtual jint getIntField(jobject, jfieldID) const;
  virtual jlong getLongField(jobject, jfieldID) const;
  virtual jfloat getFloatField(jobject, jfieldID) const;
  virtual jdouble getDoubleField(jobject, jfieldID) const;
  virtual void setObjectField(jobject, jfieldID, jobject) const;
  virtual void setBooleanField(jobject, jfieldID, jboolean b) const;
  virtual void setByteField(jobject, jfieldID, jbyte) const;
  virtual void setCharField(jobject, jfieldID, jchar) const;
  virtual void setShortField(jobject, jfieldID, jshort) const;
  virtual void setIntField(jobject, jfieldID, jint) const;
  virtual void setLongField(jobject, jfieldID, jlong) const;
  virtual void setFloatField(jobject, jfieldID, jfloat) const;
  virtual void setDoubleField(jobject, jfieldID, jdouble) const;
  virtual jfieldID getStaticFieldID(jclass, const char *, const char *) const;
  virtual jobject getStaticObjectField(jclass, jfieldID) const;
  virtual jboolean getStaticBooleanField(jclass, jfieldID) const;
  virtual jbyte getStaticByteField(jclass, jfieldID) const;
  virtual jchar getStaticCharField(jclass, jfieldID) const;
  virtual jshort getStaticShortField(jclass, jfieldID) const;
  virtual jint getStaticIntField(jclass, jfieldID) const;
  virtual jlong getStaticLongField(jclass, jfieldID) const;
  virtual jfloat getStaticFloatField(jclass, jfieldID) const;
  virtual jdouble getStaticDoubleField(jclass, jfieldID) const;
  virtual void setStaticObjectField(jclass, jfieldID, jobject) const;
  virtual void setStaticBooleanField(jclass, jfieldID, jboolean b) const;
  virtual void setStaticByteField(jclass, jfieldID, jbyte) const;
  virtual void setStaticCharField(jclass, jfieldID, jchar) const;
  virtual void setStaticShortField(jclass, jfieldID, jshort) const;
  virtual void setStaticIntField(jclass, jfieldID, jint) const;
  virtual void setStaticLongField(jclass, jfieldID, jlong) const;
  virtual void setStaticFloatField(jclass, jfieldID, jfloat) const;
  virtual void setStaticDoubleField(jclass, jfieldID, jdouble) const;

  //string.h
  virtual jstring newString(const jchar *, jsize) const;
  virtual jsize getStringLength(jstring) const;
  virtual jchar *getStringChars(jstring, jboolean *) const;
  virtual void releaseStringChars(jstring, const jchar *) const;
  virtual jstring newStringUTF(const char *) const;
  virtual jsize getStringUTFLength(jstring) const;
  virtual char *getStringUTFChars(jstring, jboolean *) const;
  virtual void releaseStringUTFChars(jstring, const char *) const;
  virtual void getStringRegion(jstring, jsize, jsize, jchar *) const;
  virtual void getStringUTFRegion(jstring, jsize, jsize, char *) const;
  virtual jchar *getStringCritical(jstring, jboolean *) const;
  virtual void releaseStringCritical(jstring, const jchar *) const;

  //array.h
  virtual void *getPrimitiveArrayCritical(jarray, jboolean *) const;
  virtual void releasePrimitiveArrayCritical(jarray, void *, jint) const;
  virtual jsize getArrayLength(jarray) const;
  virtual jobjectArray newObjectArray(jsize, jclass, jobject) const;
  virtual jobject getObjectArrayElement(jobjectArray, jsize) const;
  virtual void setObjectArrayElement(jobjectArray, jsize, jobject) const;
  virtual jbooleanArray newBooleanArray(jsize) const;
  virtual jbyteArray newByteArray(jsize) const;
  virtual jcharArray newCharArray(jsize) const;
  virtual jshortArray newShortArray(jsize) const;
  virtual jintArray newIntArray(jsize) const;
  virtual jlongArray newLongArray(jsize) const;
  virtual jfloatArray newFloatArray(jsize) const;
  virtual jdoubleArray newDoubleArray(jsize) const;
  virtual jboolean *getBooleanArrayElements(jbooleanArray, jboolean *) const;
  virtual jbyte *getByteArrayElements(jbyteArray, jboolean *) const;
  virtual jchar *getCharArrayElements(jcharArray, jboolean *) const;
  virtual jshort *getShortArrayElements(jshortArray, jboolean *) const;
  virtual jint *getIntArrayElements(jintArray, jboolean *) const;
  virtual jlong *getLongArrayElements(jlongArray, jboolean *) const;
  virtual jfloat *getFloatArrayElements(jfloatArray, jboolean *) const;
  virtual jdouble *getDoubleArrayElements(jdoubleArray, jboolean *) const;
  virtual void releaseBooleanArrayElements(jbooleanArray, jboolean *, jint) const;
  virtual void releaseByteArrayElements(jbyteArray, jbyte *, jint) const;
  virtual void releaseCharArrayElements(jcharArray, jchar *, jint) const;
  virtual void releaseShortArrayElements(jshortArray, jshort *, jint) const;
  virtual void releaseIntArrayElements(jintArray, jint *, jint) const;
  virtual void releaseLongArrayElements(jlongArray, jlong *, jint) const;
  virtual void releaseFloatArrayElements(jfloatArray, jfloat *, jint) const;
  virtual void releaseDoubleArrayElements(jdoubleArray, jdouble *, jint) const;
  virtual void getBooleanArrayRegion(jbooleanArray, jsize, jsize, jboolean *) const;
  virtual void getByteArrayRegion(jbyteArray, jsize, jsize, jbyte *) const;
  virtual void getCharArrayRegion(jcharArray, jsize, jsize, jchar *) const;
  virtual void getShortArrayRegion(jshortArray, jsize, jsize, jshort *) const;
  virtual void getIntArrayRegion(jintArray, jsize, jsize, jint *) const;
  virtual void getLongArrayRegion(jlongArray, jsize, jsize, jlong *) const;
  virtual void getFloatArrayRegion(jfloatArray, jsize, jsize, jfloat *) const;
  virtual void getDoubleArrayRegion(jdoubleArray, jsize, jsize, jdouble *) const;
  virtual void setBooleanArrayRegion(jbooleanArray, jsize, jsize, const jboolean *) const;
  virtual void setByteArrayRegion(jbyteArray, jsize, jsize, const jbyte *) const;
  virtual void setCharArrayRegion(jcharArray, jsize, jsize, const jchar *) const;
  virtual void setShortArrayRegion(jshortArray, jsize, jsize, const jshort *) const;
  virtual void setIntArrayRegion(jintArray, jsize, jsize, const jint *) const;
  virtual void setLongArrayRegion(jlongArray, jsize, jsize, const jlong *) const;
  virtual void setFloatArrayRegion(jfloatArray, jsize, jsize, const jfloat *) const;
  virtual void setDoubleArrayRegion(jdoubleArray, jsize, jsize, const jdouble *) const;

  //buffer.h
  virtual jobject newDirectByteBuffer(void *, jlong) const;
  virtual void *getDirectBufferAddress(jobject) const;
  virtual jlong getDirectBufferCapacity(jobject) const;

  //ref.h
  virtual jobjectRefType getObjectRefType(jobject) const;
  virtual jobject newGlobalRef(jobject) const;
  virtual void deleteGlobalRef(jobject) const;
  virtual void deleteLocalRef(jobject) const;
  virtual jobject newLocalRef(jobject) const;
  virtual jweak newWeakGlobalRef(jobject) const;
  virtual void deleteWeakGlobalRef(jweak) const;
 };
}