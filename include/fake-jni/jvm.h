#pragma once

#include "jni.h"
#include "jvmti.h"

#include "fake-jni/internal/util.h"
#include "fake-jni/internal/meta/meta.h"
#include "fake-jni/internal/meta/field.h"
#include "fake-jni/internal/meta/method.h"

#include "cx/templates.h"
#include "cx/indirection.h"
#include "cx/classes.h"
#include "cx/idioms.h"
#include "cx/strings.h"

#include <ffi.h>

#include <dlfcn.h>

#include <initializer_list>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <mutex>
#include <shared_mutex>

//Internal JFieldID macros
#define _ASSERT_FIELD_JNI_COMPLIANCE \
static_assert(\
 __is_base_of(JObject, T) || _CX::JniTypeBase<T>::isRegisteredType,\
 "Field type is not a valid JNI type!"\
);\
static_assert(\
 CX::ComponentTypeResolver<T>::indirectionCount == 0U,\
 "Registered field types cannot be pointers or references to a JNI type!"\
);

//Internal JMethodID macros
#define _ASSERT_JNI_FUNCTION_COMPLIANCE \
static_assert(\
 _CX::VerifyJniFunctionArguments<R>::verify(),\
 "Registered JNI functions may only return JNI types and pointers to _jobject or derived classes!"\
);\
static_assert(\
 _CX::VerifyJniFunctionArguments<Args...>::verify(),\
 "Registered JNI functions may only accept JNI types and pointers to _jobject or derived classes!"\
);

//FAKE-JNI API MACROS
#define DEFINE_JNI_TYPE(target, sig) \
namespace FakeJni::_CX {\
 template<>\
 class JniTypeBase<typename ComponentTypeResolver<target>::type> {\
 private:\
  using type = typename ComponentTypeResolver<target>::type;\
 public:\
  static constexpr const bool isRegisteredType = true;\
  static constexpr const bool isClass = __is_class(type);\
  static constexpr const char signature[] = sig;\
  static constexpr const bool hasComplexHierarchy = CastDefined<type>::value;\
 };\
}

#define DUAL_OVERLOAD_RESOLVER(_1, _2, OVERLOAD, ...) OVERLOAD

#define TRI_OVERLOAD_RESOLVER(_1, _2, _3, OVERLOAD, ...) OVERLOAD

#define _DEFINE_CLASS_NAME_1(str) \
static constexpr const char name[] = str;\
static const JClass descriptor;\
inline static const JClass * getDescriptor() noexcept {\
 return &descriptor;\
}\
virtual const JClass & getClass() const noexcept override {\
 return descriptor;\
}

#define _DEFINE_CLASS_NAME_2(str, baseClass) \
_DEFINE_CLASS_NAME_1(str)\
static constexpr const auto& base = baseClass::descriptor;

#define DEFINE_CLASS_NAME(...) \
DUAL_OVERLOAD_RESOLVER(__VA_ARGS__, _DEFINE_CLASS_NAME_2, _DEFINE_CLASS_NAME_1, ...) (__VA_ARGS__)

#define DECLARE_NATIVE_TYPE(clazz) \
namespace FakeJni::_CX {\
 template<>\
 class JniTypeBase<clazz> {\
 private:\
  static constexpr const char \
   prefix[] = "L",\
   suffix[] = ";";\
 public:\
  static constexpr const bool isRegisteredType = true;\
  static constexpr const bool isClass = true;\
  static constexpr const auto signature = CX::Concat<prefix, clazz::name, suffix>::result;\
  static constexpr const bool hasComplexHierarchy = CastDefined<clazz>::value;\
 };\
}

#define _DEFINE_NATIVE_DESCRIPTOR_2(clazz, modifiers) \
inline const FakeJni::JClass clazz::descriptor { modifiers, FakeJni::_CX::JClassBreeder<clazz> {

#define _DEFINE_NATIVE_DESCRIPTOR_1(clazz) \
_DEFINE_NATIVE_DESCRIPTOR_2(clazz, FakeJni::JClass::PUBLIC)

#define DEFINE_NATIVE_DESCRIPTOR(...) \
DUAL_OVERLOAD_RESOLVER(__VA_ARGS__, _DEFINE_NATIVE_DESCRIPTOR_2, _DEFINE_NATIVE_DESCRIPTOR_1, _) (__VA_ARGS__)

#define _BEGIN_NATIVE_DESCRIPTOR_2(clazz, modifiers) \
DECLARE_NATIVE_TYPE(clazz)\
_DEFINE_NATIVE_DESCRIPTOR_2(clazz, modifiers)

#define _BEGIN_NATIVE_DESCRIPTOR_1(clazz) \
_BEGIN_NATIVE_DESCRIPTOR_2(clazz, FakeJni::JClass::PUBLIC)

#define BEGIN_NATIVE_DESCRIPTOR(...) \
DUAL_OVERLOAD_RESOLVER(__VA_ARGS__, _BEGIN_NATIVE_DESCRIPTOR_2, _BEGIN_NATIVE_DESCRIPTOR_1, _) (__VA_ARGS__)

#define _BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR_3(primitive, descriptorName, modifiers) \
const FakeJni::JClass descriptorName { modifiers, FakeJni::_CX::JClassBreeder<primitive> {

#define _BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR_2(primitive, descriptorName) \
_BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR_3(primitive, descriptorName, FakeJni::JClass::PUBLIC)

#define BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR(...) \
TRI_OVERLOAD_RESOLVER(__VA_ARGS__, _BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR_3, _BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR_2, _) (__VA_ARGS__)

#define END_NATIVE_DESCRIPTOR }};

#define _DECLARE_NATIVE_PRIMITIVE_DESCRIPTOR_3(primitive, signature, descriptorName) \
DEFINE_JNI_TYPE(primitive, signature)\
const FakeJni::JClass descriptorName;

#define _DECLARE_NATIVE_PRIMITIVE_DESCRIPTOR_2(primitive, signature) \
_DECLARE_NATIVE_PRIMITIVE_DESCRIPTOR_3(primitive, signature, primitive##Descriptor)

#define DECLARE_NATIVE_PRIMITIVE_DESCRIPTOR(...) \
TRI_OVERLOAD_RESOLVER(__VA_ARGS__, _DECLARE_NATIVE_PRIMITIVE_DESCRIPTOR_3, _DECLARE_NATIVE_PRIMITIVE_DESCRIPTOR_2, _) (__VA_ARGS__)

namespace FakeJni {
 //Forward declare JObject to define metadata templates
 class JObject;
}

//Create template metadata for all JNI types
DEFINE_JNI_TYPE(FakeJni::JVoid, "V")
DEFINE_JNI_TYPE(FakeJni::JBoolean, "Z")
DEFINE_JNI_TYPE(FakeJni::JByte, "B")
DEFINE_JNI_TYPE(FakeJni::JChar, "C")
DEFINE_JNI_TYPE(FakeJni::JShort, "S")
DEFINE_JNI_TYPE(FakeJni::JInt, "I")
DEFINE_JNI_TYPE(FakeJni::JFloat, "F")
DEFINE_JNI_TYPE(FakeJni::JLong, "J")
DEFINE_JNI_TYPE(FakeJni::JDouble, "D")
//Must be defined before metadata templates are instantiated
DEFINE_JNI_TYPE(FakeJni::JObject, "Ljava/lang/Object;")

//JNI base
struct _jmethodID {};

//JNI base
struct _jfieldID {};

//fake-jni API declarations
namespace FakeJni {
 class Jvm;

//JNI / JVMTI WRAPPER API
 class InvokeInterface : public JNIInvokeInterface_ {
 public:
  const Jvm& vm;

  //jni/invoke/invoke.cpp
  explicit InvokeInterface(const Jvm& vm);
  explicit InvokeInterface(const InvokeInterface&) = delete;
  virtual ~InvokeInterface() = default;
  virtual InvokeInterface& operator=(const InvokeInterface& ii) noexcept;

  //jni/invoke/misc.cpp
  virtual jint getEnv(Jvm *vm, void **penv, jint version) const;
  virtual jint destroyJavaVm(Jvm *vm) const;

  //jni/invoke/thread.cpp
  virtual jint attachCurrentThread(Jvm *vm, void **penv, void *args) const;
  virtual jint detachCurrentThread(Jvm *vm) const;
  virtual jint attachCurrentThreadAsDaemon(Jvm *vm, void **penv, void *args) const;
 };

 class NativeInterface : public JNINativeInterface_ {
 private:
  //native_vararg.cpp
  static jobject _NewObject(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  static jobject _CallObjectMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  static jboolean _CallBooleanMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  static jbyte _CallByteMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  static jchar _CallCharMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  static jshort _CallShortMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  static jint _CallIntMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  static jlong _CallLongMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  static jfloat _CallFloatMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  static jdouble _CallDoubleMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  static void _CallVoidMethod(JNIEnv *env, jobject obj, jmethodID jmid, ...);
  static jobject _CallNonvirtualObjectMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  static jboolean _CallNonvirtualBooleanMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  static jbyte _CallNonvirtualByteMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  static jchar _CallNonvirtualCharMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  static jshort _CallNonvirtualShortMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  static jint _CallNonvirtualIntMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  static jlong _CallNonvirtualLongMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  static jfloat _CallNonvirtualFloatMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  static jdouble _CallNonvirtualDoubleMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  static void _CallNonvirtualVoidMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID jmid, ...);
  static jobject _CallStaticObjectMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  static jboolean _CallStaticBooleanMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  static jbyte _CallStaticByteMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  static jchar _CallStaticCharMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  static jshort _CallStaticShortMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  static jint _CallStaticIntMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  static jlong _CallStaticLongMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  static jfloat _CallStaticFloatMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  static jdouble _CallStaticDoubleMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);
  static void _CallStaticVoidMethod(JNIEnv *env, jclass clazz, jmethodID jmid, ...);

 public:
  const Jvm& vm;

  //jni/native/native.cpp
  explicit NativeInterface(const Jvm& vm);
  explicit NativeInterface(const NativeInterface &) = delete;
  virtual ~NativeInterface() = default;
  virtual NativeInterface& operator=(const NativeInterface& ni) noexcept;

  //jni/native/exception.cpp
  virtual jint throw_(jthrowable) const;
  virtual jint throwNew(jclass, const char *) const;
  virtual jthrowable exceptionOccurred() const;
  virtual void exceptionDescribe() const;
  virtual void exceptionClear() const;
  virtual jboolean exceptionCheck() const;
  virtual void fatalError(const char *) const;
  virtual jint pushLocalFrame(jint) const;
  virtual jobject popLocalFrame(jobject) const;

  //jni/native/object.cpp
  virtual jboolean isSameObject(jobject, jobject) const;
  virtual jobject allocObject(jclass) const;
//  virtual jobject newObject(jclass, jmethodID, ...) const;
  virtual jobject newObjectV(jclass, jmethodID, va_list) const;
  virtual jobject newObjectA(jclass, jmethodID, const jvalue *) const;
  virtual jclass getObjectClass(jobject) const;
  virtual jboolean isInstanceOf(jobject, jclass) const;
  virtual jclass defineClass(const char *, jobject, const jbyte *, jsize) const;
  virtual jclass findClass(const char *) const;
  virtual jclass getSuperclass(jclass) const;
  virtual jboolean isAssignableFrom(jclass, jclass) const;

  //jni/native/misc.cpp
  virtual jint ensureLocalCapacity(jint) const;
  virtual jint getVersion() const;
  virtual jint registerNatives(jclass, const JNINativeMethod *, jint) const;
  virtual jint unregisterNatives(jclass) const;
  virtual jint monitorEnter(jobject) const;
  virtual jint monitorExit(jobject) const;
  virtual jint getJavaVM(JavaVM **) const;

  //jni/native/reflect.cpp
  virtual jmethodID fromReflectedMethod(jobject) const;
  virtual jfieldID fromReflectedField(jobject) const;
  virtual jobject toReflectedMethod(jclass, jmethodID, jboolean) const;
  virtual jobject toReflectedField(jclass, jfieldID, jboolean) const;

  //jni/native/method.cpp
  virtual jmethodID getMethodID(jclass, const char *, const char *) const;
//  virtual jobject callObjectMethod(jobject, jmethodID, ...) const;
  virtual jobject callObjectMethodV(jobject, jmethodID, va_list) const;
  virtual jobject callObjectMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jboolean callBooleanMethod(jobject, jmethodID, ...) const;
  virtual jboolean callBooleanMethodV(jobject, jmethodID, va_list) const;
  virtual jboolean callBooleanMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jbyte callByteMethod(jobject, jmethodID, ...) const;
  virtual jbyte callByteMethodV(jobject, jmethodID, va_list) const;
  virtual jbyte callByteMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jchar callCharMethod(jobject, jmethodID, ...) const;
  virtual jchar callCharMethodV(jobject, jmethodID, va_list) const;
  virtual jchar callCharMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jshort callShortMethod(jobject, jmethodID, ...) const;
  virtual jshort callShortMethodV(jobject, jmethodID, va_list) const;
  virtual jshort callShortMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jint callIntMethod(jobject, jmethodID, ...) const;
  virtual jint callIntMethodV(jobject, jmethodID, va_list) const;
  virtual jint callIntMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jlong callLongMethod(jobject, jmethodID, ...) const;
  virtual jlong callLongMethodV(jobject, jmethodID, va_list) const;
  virtual jlong callLongMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jfloat callFloatMethod(jobject, jmethodID, ...) const;
  virtual jfloat callFloatMethodV(jobject, jmethodID, va_list) const;
  virtual jfloat callFloatMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jdouble callDoubleMethod(jobject, jmethodID, ...) const;
  virtual jdouble callDoubleMethodV(jobject, jmethodID, va_list) const;
  virtual jdouble callDoubleMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual void callVoidMethod(jobject, jmethodID, ...) const;
  virtual void callVoidMethodV(jobject, jmethodID, va_list) const;
  virtual void callVoidMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jobject callNonvirtualObjectMethod(jobject, jclass, jmethodID, ...) const;
  virtual jobject callNonvirtualObjectMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jobject callNonvirtualObjectMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jboolean callNonvirtualBooleanMethod(jobject, jclass, jmethodID, ...) const;
  virtual jboolean callNonvirtualBooleanMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jboolean callNonvirtualBooleanMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jbyte callNonvirtualByteMethod(jobject, jclass, jmethodID, ...) const;
  virtual jbyte callNonvirtualByteMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jbyte callNonvirtualByteMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jchar callNonvirtualCharMethod(jobject, jclass, jmethodID, ...) const;
  virtual jchar callNonvirtualCharMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jchar callNonvirtualCharMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jshort callNonvirtualShortMethod(jobject, jclass, jmethodID, ...) const;
  virtual jshort callNonvirtualShortMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jshort callNonvirtualShortMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jint callNonvirtualIntMethod(jobject, jclass, jmethodID, ...) const;
  virtual jint callNonvirtualIntMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jint callNonvirtualIntMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jlong callNonvirtualLongMethod(jobject, jclass, jmethodID, ...) const;
  virtual jlong callNonvirtualLongMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jlong callNonvirtualLongMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jfloat callNonvirtualFloatMethod(jobject, jclass, jmethodID, ...) const;
  virtual jfloat callNonvirtualFloatMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jfloat callNonvirtualFloatMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jdouble callNonvirtualDoubleMethod(jobject, jclass, jmethodID, ...) const;
  virtual jdouble callNonvirtualDoubleMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual jdouble callNonvirtualDoubleMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual void callNonvirtualVoidMethod(jobject, jclass, jmethodID, ...) const;
  virtual void callNonvirtualVoidMethodV(jobject, jclass, jmethodID, va_list) const;
  virtual void callNonvirtualVoidMethodA(jobject, jclass, jmethodID, const jvalue *) const;
  virtual jmethodID getStaticMethodID(jclass, const char *, const char *) const;
//  virtual jobject callStaticObjectMethod(jclass, jmethodID, ...) const;
  virtual jobject callStaticObjectMethodV(jclass, jmethodID, va_list) const;
  virtual jobject callStaticObjectMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jboolean callStaticBooleanMethod(jclass, jmethodID, ...) const;
  virtual jboolean callStaticBooleanMethodV(jclass, jmethodID, va_list) const;
  virtual jboolean callStaticBooleanMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jbyte callStaticByteMethod(jclass, jmethodID, ...) const;
  virtual jbyte callStaticByteMethodV(jclass, jmethodID, va_list) const;
  virtual jbyte callStaticByteMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jchar callStaticCharMethod(jclass, jmethodID, ...) const;
  virtual jchar callStaticCharMethodV(jclass, jmethodID, va_list) const;
  virtual jchar callStaticCharMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jshort callStaticShortMethod(jclass, jmethodID, ...) const;
  virtual jshort callStaticShortMethodV(jclass, jmethodID, va_list) const;
  virtual jshort callStaticShortMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jint callStaticIntMethod(jclass, jmethodID, ...) const;
  virtual jint callStaticIntMethodV(jclass, jmethodID, va_list) const;
  virtual jint callStaticIntMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jlong callStaticLongMethod(jclass, jmethodID, ...) const;
  virtual jlong callStaticLongMethodV(jclass, jmethodID, va_list) const;
  virtual jlong callStaticLongMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jfloat callStaticFloatMethod(jclass, jmethodID, ...) const;
  virtual jfloat callStaticFloatMethodV(jclass, jmethodID, va_list) const;
  virtual jfloat callStaticFloatMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jdouble callStaticDoubleMethod(jclass, jmethodID, ...) const;
  virtual jdouble callStaticDoubleMethodV(jclass, jmethodID, va_list) const;
  virtual jdouble callStaticDoubleMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual void callStaticVoidMethod(jclass, jmethodID, ...) const;
  virtual void callStaticVoidMethodV(jclass, jmethodID, va_list) const;
  virtual void callStaticVoidMethodA(jclass, jmethodID, const jvalue *) const;

  //jni/native/field.cpp
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

  //jni/native/string.cpp
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

  //jni/native/array.cpp
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

  //jni/native/buffer.cpp
  virtual jobject newDirectByteBuffer(void *, jlong) const;
  virtual void *getDirectBufferAddress(jobject) const;
  virtual jlong getDirectBufferCapacity(jobject) const;

  //jni/native/ref.cpp
  virtual jobjectRefType getObjectRefType(jobject) const;
  virtual jobject newGlobalRef(jobject) const;
  virtual void deleteGlobalRef(jobject) const;
  virtual void deleteLocalRef(jobject) const;
  virtual jobject newLocalRef(jobject) const;
  virtual jweak newWeakGlobalRef(jobject) const;
  virtual void deleteWeakGlobalRef(jweak) const;
 };

 class JvmtiInterface : public jvmtiInterface_1_ {
 private:
  //jvmti/jvmti_vararg.cpp
  static jvmtiError _SetEventNotificationMode(jvmtiEnv* env, jvmtiEventMode mode, jvmtiEvent event_type, jthread event_thread, ...);

 public:
  const Jvm& vm;

  //jvmti/jvmti.cpp
  explicit JvmtiInterface(const Jvm& vm);
  explicit JvmtiInterface(const JvmtiInterface&) = delete;
  virtual ~JvmtiInterface() = default;
  virtual JvmtiInterface& operator=(const JvmtiInterface& ji) noexcept;

  //jvmti/thread.cpp
  virtual jvmtiError getAllThreads(jvmtiEnv* env, jint* threads_count_ptr, jthread** threads_ptr) const;
  virtual jvmtiError suspendThread(jvmtiEnv* env, jthread thread) const;
  virtual jvmtiError resumeThread(jvmtiEnv* env, jthread thread) const;
  virtual jvmtiError stopThread(jvmtiEnv* env, jthread thread, jobject exception) const;
  virtual jvmtiError interruptThread(jvmtiEnv* env, jthread thread) const;
  virtual jvmtiError getThreadInfo(jvmtiEnv* env, jthread thread, jvmtiThreadInfo* info_ptr) const;
  virtual jvmtiError runAgentThread(jvmtiEnv* env, jthread thread, jvmtiStartFunction proc, const void* arg, jint priority) const;
  virtual jvmtiError getTopThreadGroups(jvmtiEnv* env, jint* group_count_ptr, jthreadGroup** groups_ptr) const;
  virtual jvmtiError getThreadGroupInfo(jvmtiEnv* env, jthreadGroup group, jvmtiThreadGroupInfo* info_ptr) const;
  virtual jvmtiError getThreadGroupChildren(jvmtiEnv* env, jthreadGroup group, jint* thread_count_ptr, jthread** threads_ptr, jint* group_count_ptr, jthreadGroup** groups_ptr) const;
  virtual jvmtiError getFrameCount(jvmtiEnv* env, jthread thread, jint* count_ptr) const;
  virtual jvmtiError getThreadState(jvmtiEnv* env, jthread thread, jint* thread_state_ptr) const;
  virtual jvmtiError getCurrentThread(jvmtiEnv* env, jthread* thread_ptr) const;
  virtual jvmtiError suspendThreadList(jvmtiEnv* env, jint request_count, const jthread* request_list, jvmtiError* results) const;
  virtual jvmtiError resumeThreadList(jvmtiEnv* env, jint request_count, const jthread* request_list, jvmtiError* results) const;
  virtual jvmtiError getThreadListStackTraces(jvmtiEnv* env, jint thread_count, const jthread* thread_list, jint max_frame_count, jvmtiStackInfo** stack_info_ptr) const;
  virtual jvmtiError getThreadLocalStorage(jvmtiEnv* env, jthread thread, void** data_ptr) const;
  virtual jvmtiError setThreadLocalStorage(jvmtiEnv* env, jthread thread, const void* data) const;
  virtual jvmtiError getCurrentThreadCpuTimerInfo(jvmtiEnv* env, jvmtiTimerInfo* info_ptr) const;
  virtual jvmtiError getCurrentThreadCpuTime(jvmtiEnv* env, jlong* nanos_ptr) const;
  virtual jvmtiError getThreadCpuTimerInfo(jvmtiEnv* env, jvmtiTimerInfo* info_ptr) const;
  virtual jvmtiError getThreadCpuTime(jvmtiEnv* env, jthread thread, jlong* nanos_ptr) const;

  //jvmti/monitor.cpp
  virtual jvmtiError getOwnedMonitorInfo(jvmtiEnv* env, jthread thread, jint* owned_monitor_count_ptr, jobject** owned_monitors_ptr) const;
  virtual jvmtiError getCurrentContendedMonitor(jvmtiEnv* env, jthread thread, jobject* monitor_ptr) const;
  virtual jvmtiError createRawMonitor(jvmtiEnv* env, const char* name, jrawMonitorID* monitor_ptr) const;
  virtual jvmtiError destroyRawMonitor(jvmtiEnv* env, jrawMonitorID monitor) const;
  virtual jvmtiError rawMonitorEnter(jvmtiEnv* env, jrawMonitorID monitor) const;
  virtual jvmtiError rawMonitorExit(jvmtiEnv* env, jrawMonitorID monitor) const;
  virtual jvmtiError rawMonitorWait(jvmtiEnv* env, jrawMonitorID monitor, jlong millis) const;
  virtual jvmtiError rawMonitorNotify(jvmtiEnv* env, jrawMonitorID monitor) const;
  virtual jvmtiError rawMonitorNotifyAll(jvmtiEnv* env, jrawMonitorID monitor) const;
  virtual jvmtiError getObjectMonitorUsage(jvmtiEnv* env, jobject object, jvmtiMonitorUsage* info_ptr) const;
  virtual jvmtiError getOwnedMonitorStackDepthInfo(jvmtiEnv* env, jthread thread, jint* monitor_info_count_ptr, jvmtiMonitorStackDepthInfo** monitor_info_ptr) const;

  //jvmti/class.cpp
  virtual jvmtiError getClassSignature(jvmtiEnv* env, jclass klass, char** signature_ptr, char** generic_ptr) const;
  virtual jvmtiError getClassStatus(jvmtiEnv* env, jclass klass, jint* status_ptr) const;
  virtual jvmtiError getSourceFileName(jvmtiEnv* env, jclass klass, char** source_name_ptr) const;
  virtual jvmtiError getClassModifiers(jvmtiEnv* env, jclass klass, jint* modifiers_ptr) const;
  virtual jvmtiError getClassMethods(jvmtiEnv* env, jclass klass, jint* method_count_ptr, jmethodID** methods_ptr) const;
  virtual jvmtiError getClassFields(jvmtiEnv* env, jclass klass, jint* field_count_ptr, jfieldID** fields_ptr) const;
  virtual jvmtiError getImplementedInterfaces(jvmtiEnv* env, jclass klass, jint* interface_count_ptr, jclass** interfaces_ptr) const;
  virtual jvmtiError isInterface(jvmtiEnv* env, jclass klass, jboolean* is_interface_ptr) const;
  virtual jvmtiError isArrayClass(jvmtiEnv* env, jclass klass, jboolean* is_array_class_ptr) const;
  virtual jvmtiError getClassLoader(jvmtiEnv* env, jclass klass, jobject* classloader_ptr) const;
  virtual jvmtiError isModifiableClass(jvmtiEnv* env, jclass klass, jboolean* is_modifiable_class_ptr) const;
  virtual jvmtiError getLoadedClasses(jvmtiEnv* env, jint* class_count_ptr, jclass** classes_ptr) const;
  virtual jvmtiError getClassLoaderClasses(jvmtiEnv* env, jobject initiating_loader, jint* class_count_ptr, jclass** classes_ptr) const;
  virtual jvmtiError redefineClasses(jvmtiEnv* env, jint class_count, const jvmtiClassDefinition* class_definitions) const;
  virtual jvmtiError getClassVersionNumbers(jvmtiEnv* env, jclass klass, jint* minor_version_ptr, jint* major_version_ptr) const;
  virtual jvmtiError getConstantPool(jvmtiEnv* env, jclass klass, jint* constant_pool_count_ptr, jint* constant_pool_byte_count_ptr, unsigned char** constant_pool_bytes_ptr) const;
  virtual jvmtiError addToBootstrapClassLoaderSearch(jvmtiEnv* env, const char* segment) const;
  virtual jvmtiError addToSystemClassLoaderSearch(jvmtiEnv* env, const char* segment) const;
  virtual jvmtiError retransformClasses(jvmtiEnv* env, jint class_count, const jclass* classes) const;

  //jvmti/field.cpp
  virtual jvmtiError getFieldName(jvmtiEnv* env, jclass klass, jfieldID field, char** name_ptr, char** signature_ptr, char** generic_ptr) const;
  virtual jvmtiError getFieldDeclaringClass(jvmtiEnv* env, jclass klass, jfieldID field, jclass* declaring_class_ptr) const;
  virtual jvmtiError getFieldModifiers(jvmtiEnv* env, jclass klass, jfieldID field, jint* modifiers_ptr) const;
  virtual jvmtiError isFieldSynthetic(jvmtiEnv* env, jclass klass, jfieldID field, jboolean* is_synthetic_ptr) const;
  virtual jvmtiError setFieldAccessWatch(jvmtiEnv* env, jclass klass, jfieldID field) const;
  virtual jvmtiError clearFieldAccessWatch(jvmtiEnv* env, jclass klass, jfieldID field) const;
  virtual jvmtiError setFieldModificationWatch(jvmtiEnv* env, jclass klass, jfieldID field) const;
  virtual jvmtiError clearFieldModificationWatch(jvmtiEnv* env, jclass klass, jfieldID field) const;

  //jvmti/method.cpp
  virtual jvmtiError getMethodName(jvmtiEnv* env, jmethodID method, char** name_ptr, char** signature_ptr, char** generic_ptr) const;
  virtual jvmtiError getMethodDeclaringClass(jvmtiEnv* env, jmethodID method, jclass* declaring_class_ptr) const;
  virtual jvmtiError getMethodModifiers(jvmtiEnv* env, jmethodID method, jint* modifiers_ptr) const;
  virtual jvmtiError getMaxLocals(jvmtiEnv* env, jmethodID method, jint* max_ptr) const;
  virtual jvmtiError getArgumentsSize(jvmtiEnv* env, jmethodID method, jint* size_ptr) const;
  virtual jvmtiError getLineNumberTable(jvmtiEnv* env, jmethodID method, jint* entry_count_ptr, jvmtiLineNumberEntry** table_ptr) const;
  virtual jvmtiError getMethodLocation(jvmtiEnv* env, jmethodID method, jlocation* start_location_ptr, jlocation* end_location_ptr) const;
  virtual jvmtiError getLocalVariableTable(jvmtiEnv* env, jmethodID method, jint* entry_count_ptr, jvmtiLocalVariableEntry** table_ptr) const;
  virtual jvmtiError setNativeMethodPrefix(jvmtiEnv* env, const char* prefix) const;
  virtual jvmtiError setNativeMethodPrefixes(jvmtiEnv* env, jint prefix_count, char** prefixes) const;
  virtual jvmtiError getBytecodes(jvmtiEnv* env, jmethodID method, jint* bytecode_count_ptr, unsigned char** bytecodes_ptr) const;
  virtual jvmtiError isMethodNative(jvmtiEnv* env, jmethodID method, jboolean* is_native_ptr) const;
  virtual jvmtiError isMethodSynthetic(jvmtiEnv* env, jmethodID method, jboolean* is_synthetic_ptr) const;
  virtual jvmtiError isMethodObsolete(jvmtiEnv* env, jmethodID method, jboolean* is_obsolete_ptr) const;

  //jvmti/misc.cpp
  virtual jvmtiError setEventNotificationMode(jvmtiEnv* env, jvmtiEventMode mode, jvmtiEvent event_type, jthread event_thread) const;
  virtual jvmtiError allocate(jvmtiEnv* env, jlong size, unsigned char** mem_ptr) const;
  virtual jvmtiError deallocate(jvmtiEnv* env, unsigned char* mem) const;
  virtual jvmtiError setBreakpoint(jvmtiEnv* env, jmethodID method, jlocation location) const;
  virtual jvmtiError clearBreakpoint(jvmtiEnv* env, jmethodID method, jlocation location) const;
  virtual jvmtiError getObjectHashCode(jvmtiEnv* env, jobject object, jint* hash_code_ptr) const;
  virtual jvmtiError popFrame(jvmtiEnv* env, jthread thread) const;
  virtual jvmtiError forceEarlyReturnObject(jvmtiEnv* env, jthread thread, jobject value) const;
  virtual jvmtiError forceEarlyReturnInt(jvmtiEnv* env, jthread thread, jint value) const;
  virtual jvmtiError forceEarlyReturnLong(jvmtiEnv* env, jthread thread, jlong value) const;
  virtual jvmtiError forceEarlyReturnFloat(jvmtiEnv* env, jthread thread, jfloat value) const;
  virtual jvmtiError forceEarlyReturnDouble(jvmtiEnv* env, jthread thread, jdouble value) const;
  virtual jvmtiError forceEarlyReturnVoid(jvmtiEnv* env, jthread thread) const;
  virtual jvmtiError getVersionNumber(jvmtiEnv* env, jint* version_ptr) const;
  virtual jvmtiError getCapabilities(jvmtiEnv* env, jvmtiCapabilities* capabilities_ptr) const;
  virtual jvmtiError getSourceDebugExtension(jvmtiEnv* env, jclass klass, char** source_debug_extension_ptr) const;
  virtual jvmtiError getTag(jvmtiEnv* env, jobject object, jlong* tag_ptr) const;
  virtual jvmtiError setTag(jvmtiEnv* env, jobject object, jlong tag) const;
  virtual jvmtiError forceGarbageCollection(jvmtiEnv* env) const;
  virtual jvmtiError iterateOverObjectsReachableFromObject(jvmtiEnv* env, jobject object, jvmtiObjectReferenceCallback object_reference_callback, const void* user_data) const;
  virtual jvmtiError iterateOverReachableObjects(jvmtiEnv* env, jvmtiHeapRootCallback heap_root_callback, jvmtiStackReferenceCallback stack_ref_callback, jvmtiObjectReferenceCallback object_ref_callback, const void* user_data) const;
  virtual jvmtiError iterateOverHeap(jvmtiEnv* env, jvmtiHeapObjectFilter object_filter, jvmtiHeapObjectCallback heap_object_callback, const void* user_data) const;
  virtual jvmtiError iterateOverInstancesOfClass(jvmtiEnv* env, jclass klass, jvmtiHeapObjectFilter object_filter, jvmtiHeapObjectCallback heap_object_callback, const void* user_data) const;
  virtual jvmtiError getObjectsWithTags(jvmtiEnv* env, jint tag_count, const jlong* tags, jint* count_ptr, jobject** object_result_ptr, jlong** tag_result_ptr) const;
  virtual jvmtiError followReferences(jvmtiEnv* env, jint heap_filter, jclass klass, jobject initial_object, const jvmtiHeapCallbacks* callbacks, const void* user_data) const;
  virtual jvmtiError iterateThroughHeap(jvmtiEnv* env, jint heap_filter, jclass klass, const jvmtiHeapCallbacks* callbacks, const void* user_data) const;
  virtual jvmtiError setJNIFunctionTable(jvmtiEnv* env, const jniNativeInterface* function_table) const;
  virtual jvmtiError getJNIFunctionTable(jvmtiEnv* env, jniNativeInterface** function_table) const;
  virtual jvmtiError setEventCallbacks(jvmtiEnv* env, const jvmtiEventCallbacks* callbacks, jint size_of_callbacks) const;
  virtual jvmtiError generateEvents(jvmtiEnv* env, jvmtiEvent event_type) const;
  virtual jvmtiError getExtensionFunctions(jvmtiEnv* env, jint* extension_count_ptr, jvmtiExtensionFunctionInfo** extensions) const;
  virtual jvmtiError getExtensionEvents(jvmtiEnv* env, jint* extension_count_ptr, jvmtiExtensionEventInfo** extensions) const;
  virtual jvmtiError setExtensionEventCallback(jvmtiEnv* env, jint extension_event_index, jvmtiExtensionEvent callback) const;
  virtual jvmtiError disposeEnvironment(jvmtiEnv* env) const;
  virtual jvmtiError getErrorName(jvmtiEnv* env, jvmtiError error, char** name_ptr) const;
  virtual jvmtiError getJLocationFormat(jvmtiEnv* env,jvmtiJlocationFormat* format_ptr) const;
  virtual jvmtiError getSystemProperties(jvmtiEnv* env, jint* count_ptr, char*** property_ptr) const;
  virtual jvmtiError getSystemProperty(jvmtiEnv* env, const char* property, char** value_ptr) const;
  virtual jvmtiError setSystemProperty(jvmtiEnv* env, const char* property, const char* value) const;
  virtual jvmtiError getPhase(jvmtiEnv* env, jvmtiPhase* phase_ptr) const;
  virtual jvmtiError getTimerInfo(jvmtiEnv* env, jvmtiTimerInfo* info_ptr) const;
  virtual jvmtiError getTime(jvmtiEnv* env, jlong* nanos_ptr) const;
  virtual jvmtiError getPotentialCapabilities(jvmtiEnv* env, jvmtiCapabilities* capabilities_ptr) const;
  virtual jvmtiError addCapabilities(jvmtiEnv* env, const jvmtiCapabilities* capabilities_ptr) const;
  virtual jvmtiError relinquishCapabilities(jvmtiEnv* env, const jvmtiCapabilities* capabilities_ptr) const;
  virtual jvmtiError getAvailableProcessors(jvmtiEnv* env, jint* processor_count_ptr) const;
  virtual jvmtiError getEnvironmentLocalStorage(jvmtiEnv* env, void** data_ptr) const;
  virtual jvmtiError setEnvironmentLocalStorage(jvmtiEnv* env, const void* data) const;
  virtual jvmtiError setVerboseFlag(jvmtiEnv* env, jvmtiVerboseFlag flag, jboolean value) const;
  virtual jvmtiError getObjectSize(jvmtiEnv* env, jobject object, jlong* size_ptr) const;
  virtual jvmtiError getLocalInstance(jvmtiEnv* env, jthread thread, jint depth, jobject* value_ptr) const;
  virtual jvmtiError getFrameLocation(jvmtiEnv* env, jthread thread, jint depth, jmethodID* method_ptr, jlocation* location_ptr) const;
  virtual jvmtiError notifyFramePop(jvmtiEnv* env, jthread thread, jint depth) const;
  virtual jvmtiError getLocalObject(jvmtiEnv* env, jthread thread, jint depth, jint slot, jobject* value_ptr) const;
  virtual jvmtiError getLocalInt(jvmtiEnv* env, jthread thread, jint depth, jint slot, jint* value_ptr) const;
  virtual jvmtiError getLocalLong(jvmtiEnv* env, jthread thread, jint depth, jint slot, jlong* value_ptr) const;
  virtual jvmtiError getLocalFloat(jvmtiEnv* env, jthread thread, jint depth, jint slot, jfloat* value_ptr) const;
  virtual jvmtiError getLocalDouble(jvmtiEnv* env, jthread thread, jint depth, jint slot, jdouble* value_ptr) const;
  virtual jvmtiError setLocalObject(jvmtiEnv* env, jthread thread, jint depth, jint slot, jobject value) const;
  virtual jvmtiError setLocalInt(jvmtiEnv* env, jthread thread, jint depth, jint slot, jint value) const;
  virtual jvmtiError setLocalLong(jvmtiEnv* env, jthread thread, jint depth, jint slot, jlong value) const;
  virtual jvmtiError setLocalFloat(jvmtiEnv* env, jthread thread, jint depth, jint slot, jfloat value) const;
  virtual jvmtiError setLocalDouble(jvmtiEnv* env, jthread thread, jint depth, jint slot, jdouble value) const;
  virtual jvmtiError getAllStackTraces(jvmtiEnv* env, jint max_frame_count, jvmtiStackInfo** stack_info_ptr, jint* thread_count_ptr) const;
  virtual jvmtiError getStackTrace(jvmtiEnv* env, jthread thread, jint start_depth, jint max_frame_count, jvmtiFrameInfo* frame_buffer, jint* count_ptr) const;
 };

 class JniEnv : public JNIEnv {
 public:
  const Jvm& vm;

 public:
  //jni/jni_env.cpp
  explicit JniEnv(const Jvm& vm) noexcept;
  explicit JniEnv(const JniEnv&) = delete;
  virtual ~JniEnv() = default;
  virtual JniEnv& operator=(const JniEnv& env) noexcept;
  virtual const Jvm& getVM() const noexcept;
 };

 class JvmtiEnv : public jvmtiEnv {
 public:
  const Jvm& vm;

  explicit JvmtiEnv(const Jvm& vm) noexcept;
  explicit JvmtiEnv(const JvmtiEnv&) = delete;
  virtual ~JvmtiEnv() = default;
  virtual JvmtiEnv& operator=(const JvmtiEnv& env) noexcept;
  virtual const Jvm& getVM() const noexcept;
 };

 using dlopen_t = void * (* const)(const char * filename, int flags);
 using dlsym_t = void * (* const)(void * handle, const char * symbol);
 using dlclose_t = int (* const)(void * handle);
 using dlerror_t = char * (* const)();

 class LibraryOptions final {
 public:
  dlopen_t dlopen_p;
  dlsym_t dlsym_p;
  dlclose_t dlclose_p;
  dlerror_t dlerror_p;

  LibraryOptions(
   dlopen_t dlopen_p = &dlopen,
   dlsym_t dlsym_p = &dlsym,
   dlclose_t dlclose_p = &dlclose,
   dlerror_t dlerror_p = &dlerror
  ) :
   dlopen_p(dlopen_p),
   dlsym_p(dlsym_p),
   dlclose_p(dlclose_p),
   dlerror_p(dlerror_p)
  {}
 };

 //Represents both JNI Agents and JNI Libraries (can only be bound to a single DefaultJvm instance at a time)
 //Dynamic Agents use Agent_OnAttach, Agent_OnLoad and Agent_OnUnload
 //Static Agents use Agent_OnAttach_L, Agent_OnLoad_L, Agent_OnUnload_L
 //Dynamic Libraries use JNI_OnLoad and JNI_OnUnload
 //Static Libraries use JNI_OnLoad_L and JNI_OnUnload_L
 class Library final {
 private:
  void * handle = nullptr;
  jint (* JNI_OnLoad_)(JavaVM * vm, void * reserved) = nullptr;
  void (* JNI_OnUnload_)(JavaVM * vm, void * reserved) = nullptr;
  jint (* Agent_OnAttach_)(JavaVM * vm, char * options, void * reserved) = nullptr;
  jint (* Agent_OnLoad_)(JavaVM * vm, char * options, void * reserved) = nullptr;
  void (* Agent_OnUnload_)(JavaVM * vm, char * options, void * reserved) = nullptr;

 public:
  const Jvm& vm;
  const std::string path;
  const LibraryOptions options;

  Library(const Jvm& vm, std::string path, LibraryOptions options);
  ~Library();

  void * lsym(const char * symbol) const;
  bool agentBound() const;
  bool jniBound() const;
  JInt agentAttach(char *options) const;
  JInt agentLoad(char *options) const;
  void agentUnload(char *options) const;
  jint jniLoad() const;
  void jniUnload() const;
 };

 class JClass;

 //JNI _jobject and java/lang/Object implementation
 class JObject : public _jobject {
 public:
  //Internal fake-jni native class metadata
  //DEFINE_CLASS_NAME cant be used since this is the virtual base
  static constexpr const char name[] = "java/lang/Object";
  static const JClass descriptor;

  inline static const JClass * getDescriptor() noexcept {
   return &descriptor;
  }

  constexpr JObject() = default;
  virtual ~JObject() = default;
  virtual const JClass & getClass() const noexcept;
 };

 //fake-jni implementation
 class JFieldID final : public _jfieldID {
 private:
  using staticProp_t = void * const;
  using memberProp_t = int (_CX::AnyClass::* const);

  const bool isStatic;
  const uint32_t modifiers;
  union {
   staticProp_t staticProp;
   memberProp_t memberProp;
  };
  const char
   * const name,
   * const signature;

  void
   (* const proxyGetFunc)(),
   (* const proxySetFunc)();

 public:
  enum Modifiers: uint32_t {
   PUBLIC = 1,
   PRIVATE = 2,
   PROTECTED = 4,
   STATIC = 8,
   FINAL = 16,
   VOLATILE = 64,
   TRANSIENT = 128,
   SYNTHETIC = 4096,
   ENUM = 16384
  };

  //Constructor for member fields
  template<typename T, typename M>
  JFieldID(T M::* member, const char * name, uint32_t modifiers) noexcept;
  //Constructor for non-member fields
  template<typename T>
  JFieldID(T * staticMember, const char * name, uint32_t modifiers) noexcept;

  inline const char * getName() const noexcept {
   return name;
  }

  inline const char * getSignature() const noexcept {
   return signature;
  }

  inline uint32_t getModifiers() const noexcept {
   return modifiers;
  }

  bool operator ==(JFieldID &fid) const noexcept;
  template<typename T>
  T get(JObject * obj);
  void set(JObject * obj, void * value);
 };

 //fake-jni implementation
 //TODO protect the mutable fields in JNINativeMethod
// class JMethodID final : public _jmethodID, private JNINativeMethod {
 class JMethodID final : public _jmethodID, public JNINativeMethod {
 public:
  using static_func_t = void (* const)();
  using member_func_t = void (_CX::AnyClass::* const)();
  using void_func_t = void (*)();

  enum Type {
   STATIC_FUNC,
   MEMBER_FUNC,
   REGISTER_NATIVES_FUNC
  };

  const Type type;

 private:
  union {
   //Functions registered through fake-jni
   struct {
    //Functions registered through registerNatives do not have modifiers
    const uint32_t modifiers;
    union {
     static_func_t staticFunc;
     member_func_t memberFunc;
    };
    static_func_t
     proxyFuncV,
     proxyFuncA;
   };
   //Functions registered through registerNatives
   struct {
    ffi_cif * descriptor;
    void_func_t
     * const resolvers,
     * const deallocators;
   };
  };

  //Default case for va_list
  //TODO this is nasty, there is no type checking for `T` on `JMethodID::invoke<R, T>` because of va_list
  // fix this
  template<typename>
  [[gnu::always_inline]]
  static_func_t getFunctionProxy() const {
   return proxyFuncV;
  }

  static /*const*/ char * verifyName(const char * name);
  static /*const*/ char * verifySignature(const char * sig);
  static ffi_cif * getFfiPrototype(const char * signature, const char * name);

  template<typename R, typename A>
  R internalInvoke(JavaVM * vm, void * clazzOrInst, A args) const;

 public:
  enum Modifiers : uint32_t {
   PUBLIC = 1,
   PRIVATE = 2,
   PROTECTED = 4,
   STATIC = 8,
   FINAL = 16,
   SYNCHRONIZED = 32,
   BRIDGE = 64,
   VARARGS = 128,
   NATIVE = 256,
   ABSTRACT = 1024,
   STRICT = 2048,
   SYNTHETIC = 4096
  };

  //Constructor for delegate constructors
  template<typename R, typename... Args>
  JMethodID(R (* const func)(Args...), uint32_t modifiers) noexcept;
  //constructor for static functions
  template<typename R, typename... Args>
  JMethodID(R (* const func)(Args...), const char * name, uint32_t modifiers) noexcept;
  //Constructor for member methods
  template<typename T, typename R, typename... Args>
  JMethodID(R (T::* const func)(Args...), const char * name, uint32_t modifiers) noexcept;
  //Constructor for RegisterNatives
  JMethodID(const JNINativeMethod * method);
  ~JMethodID();

  inline const char * getName() const noexcept {
   return name;
  }

  inline const char * getSignature() const noexcept {
   return signature;
  }

  inline uint32_t getModifiers() const noexcept {
   switch (type) {
    case REGISTER_NATIVES_FUNC: return 0;
    default: return modifiers;
   }
  }

  bool operator ==(const JMethodID& mid) const noexcept;
  bool operator ==(JNINativeMethod*& mid) const;
  template<typename R, typename A>
  R invoke(JavaVM * vm, void * clazzOrInst, A args) const;
  template<typename R, typename A>
  R invoke(JavaVM * const vm, JClass * const clazz, void * const inst, A args) const;
 };

 //Template glue code for native class registration
 namespace _CX {
  class ClassDescriptorElement;

//  template<typename T, auto IsClass = __is_class(T)>
  template<typename T, auto IsClass = std::is_class<T>::value>
  struct JClassBreeder;

  template<typename T>
  struct JClassBreeder<T, nullptr> {
   template<typename A>
   using constructor_func_t = JObject * (*)(JavaVM * vm, const char * signature, A args);

   const std::initializer_list<ClassDescriptorElement> descriptorElements;
   const JClass& parent = []() constexpr noexcept -> const JClass& {
    if constexpr(_CX::BaseDefined<T>::value) {
     return T::base;
    }
    return JObject::descriptor;
   }();

   constexpr JClassBreeder(decltype(descriptorElements) descriptorElements) noexcept;
   constexpr JClassBreeder() noexcept = default;
  };

  template<typename T>
  struct JClassBreeder<T, true> final : JClassBreeder<T, nullptr> {
   using JClassBreeder<T, nullptr>::JClassBreeder;

   template<typename A>
   static constexpr typename JClassBreeder<T, nullptr>::template constructor_func_t<A> constructorPredicate() noexcept;

   static constexpr void assertCompliance() noexcept;
  };

  template<typename T>
  struct JClassBreeder<T, false> final : JClassBreeder<T, nullptr> {
   using JClassBreeder<T, nullptr>::JClassBreeder;

   template<typename A>
   static constexpr typename JClassBreeder<T, nullptr>::template constructor_func_t<A> constructorPredicate() noexcept;

   static constexpr void assertCompliance() noexcept;
  };
 }

 class JClass : public JObject, public _jclass {
 private:
  JObject
   * (* const constructV)(JavaVM *, const char *, va_list),
   * (* const constructA)(JavaVM *, const char *, const jvalue *);

  const bool isArbitrary;
  const char * const className;

 public:
  //Internal fake-jni native class metadata
  DEFINE_CLASS_NAME("java/lang/Class")

  using cast = CX::ExplicitCastGenerator<JClass, JObject, _jobject>;

  const uint32_t modifiers;
  const JClass& parent;

  //Property associations
  AllocStack<JMethodID *> functions;
  AllocStack<JFieldID *> fields;

  enum Modifiers : uint32_t {
   PUBLIC = 1,
   PRIVATE = 2,
   PROTECTED = 4,
   STATIC = 8,
   FINAL = 16,
   INTERFACE = 512,
   ABSTRACT = 1024,
   SYNTHETIC = 4096,
   ANNOTATION = 8192,
   ENUM = 16384
  };

  explicit JClass(JClass &) = delete;
  template<typename T>
  explicit JClass(uint32_t modifiers, _CX::JClassBreeder<T, true> breeder) noexcept;
  template<typename T>
  explicit JClass(uint32_t modifiers, _CX::JClassBreeder<T, false> breeder) noexcept;
  explicit JClass(const char * name, uint32_t modifiers = PUBLIC) noexcept;
  virtual ~JClass() = default;

  bool registerMethod(JMethodID * mid) const;
  bool unregisterMethod(JMethodID * mid) const noexcept;
  JMethodID * getMethod(const char * sig, const char * name) const noexcept;
  const AllocStack<JMethodID *>& getMethods() const noexcept;
  bool registerField(JFieldID * fid) const noexcept;
  bool unregisterField(JFieldID * fid) const noexcept;
  JFieldID * getField(const char * name) const noexcept;
  const AllocStack<JFieldID *>& getFields() const noexcept;
  virtual const char * getName() const noexcept;
  //Object construction for c-varargs
  JObject * newInstance(JavaVM * vm, const char * signature, va_list list) const;
  //Object construction for jvalue arrays
  JObject * newInstance(JavaVM * vm, const char * signature, const jvalue * values) const;
 };

 //FAKE-JNI USER API
 //TODO all JVM calls should be blocking until execution completes to prevent race conditions from emerging
 class Jvm : public JavaVM {
 public:
  const char * const uuid;

 private:
  FILE * const log;
  InvokeInterface& invoke;
  NativeInterface& native;
  JvmtiInterface& jvmti;
  JniEnv& jniEnv;
  JvmtiEnv& jvmtiEnv;

  //TODO on the first invocation of any JNI, JNIEnv or JVMTI functions, set this flag to true
  bool running;

  std::vector<Library *> libraries;
  std::vector<JClass *> classes;
  //TODO if classloaders are ever implemented, this property will be handled by the ClassLoader model
//  AllocStack<JObject *> instances{true};
  std::map<const JClass *, AllocStack<JObject *>> instances;
  mutable std::shared_mutex instances_mutex;

 protected:
  static AllocStack<Jvm *> vms;

 public:
  explicit Jvm(FILE * log = stdout);
  virtual ~Jvm();

  //Other properties
  virtual FILE * getLog() const;
  virtual void registerDefaultClasses();
  virtual const char * generateJvmUuid() noexcept;

  //Interface and Env api
  virtual InvokeInterface& createDefaultInvokeInterface() const;
  virtual void setInvokeInterface(const InvokeInterface& ii);
  virtual InvokeInterface& getInvokeInterface() const;

  virtual NativeInterface& createDefaultNativeInterface() const;
  virtual void setNativeInterface(const NativeInterface& ni);
  virtual NativeInterface& getNativeInterface() const;

  virtual JvmtiInterface& createDefaultJvmtiInterface() const;
  virtual void setJvmtiInterface(const JvmtiInterface& jvmti);
  virtual JvmtiInterface& getJvmtiInterface() const;

  virtual JniEnv& createDefaultJniEnv() const;
  virtual void setJniEnv(const JniEnv env);
  virtual JniEnv& getJniEnv() const;

  virtual JvmtiEnv& createDefaultJvmtiEnv() const;
  virtual void setJvmtiEnv(const JvmtiEnv& env);
  virtual JvmtiEnv& getJvmtiEnv() const;

  const AllocStack<JObject *>& operator[](const JClass* clazz) const;
  AllocStack<JObject *>& operator[](const JClass* clazz);
  const decltype(instances)& getAllInstances() const;
  void pushInstance(JObject *inst);

  inline virtual bool isRunning() const {
   return running;
  }

  template<typename T>
  bool registerClass();
  template<typename T>
  bool unregisterClass();
  virtual bool registerClass(const JClass * clazz);
  virtual bool unregisterClass(const JClass * clazz);
  virtual const JClass * findClass(const char * name) const;
  virtual Library * getLibrary(const std::string & path) const;
  //Needs to be const to allow attaching agents / JNI modules at runtime
  virtual Library * attachLibrary(
   const std::string &rpath,
   const std::string &options = "",
   LibraryOptions loptions = {&dlopen, &dlsym, &dlclose}
  ) const;
  virtual bool removeLibrary(const std::string& path, const std::string& options) const;
  virtual void start();
  virtual void destroy();
 };

 //Template glue code for native class registration
 namespace _CX {
  class ClassDescriptorElement {
  public:
   const std::function<bool (JClass * const)> processHook;

   //Constructor for functions
   template<auto F>
   constexpr ClassDescriptorElement(Function<F> function, const char * const name, uint32_t modifiers = JMethodID::PUBLIC) noexcept :
    processHook([=](JClass * const clazz) -> bool {
     return clazz->registerMethod(new JMethodID(function.function, name, modifiers));
    })
   {}

   //Constructor for fields
   template<auto F>
   constexpr ClassDescriptorElement(Field<F> field, const char * const name, uint32_t modifiers = JFieldID::PUBLIC) noexcept :
    processHook([=](JClass * const clazz) -> bool {
     return clazz->registerField(new JFieldID(field.field, name, modifiers));
    })
   {}

   //Constructor for constructors
   template<typename T, typename... Args>
   constexpr ClassDescriptorElement(Constructor<T, Args...> constructor, uint32_t modifiers = JMethodID::PUBLIC) noexcept :
    processHook([=](JClass * const clazz) -> bool {
     return clazz->registerMethod(new JMethodID(decltype(constructor)::construct, modifiers));
    })
   {}

   [[gnu::always_inline]]
   inline bool process(JClass * const clazz) const {
    return processHook(clazz);
   }
  };

  template<typename T>
  class BaseDefined<T, CX::void_t<decltype(T::base)>> : public CX::true_type {
  private:
   struct BaseTypeAssertion {
    constexpr BaseTypeAssertion() noexcept {
     static_assert(
      CX::IsSame<JClass, typename CX::ComponentTypeResolver<decltype(T::base)>::type>::value,
      "The member field 'base' is reserved for internal fake-jni usage!"
     );
    }
   };

   //Will be constructed if T::base is defined
   //Static assertion will fail if base is not a compliant type
   static constexpr const BaseTypeAssertion assert{};
  };
 }
}

//Create type metadata for JClass
DECLARE_NATIVE_TYPE(FakeJni::JClass)

//fake-jni API definitions
namespace FakeJni {
 //JFieldID template members
 template<typename T, typename M>
 JFieldID::JFieldID(T M::* const member, const char * const name, const uint32_t modifiers) noexcept :
  _jfieldID(),
  isStatic(false),
  modifiers(modifiers),
  memberProp((memberProp_t)member),
  name(name),
  signature(_CX::JniTypeBase<T>::signature),
  proxyGetFunc((void (*)())&_CX::FieldAccessor<T (M::*)>::get),
  proxySetFunc((void (*)())&_CX::FieldAccessor<T (M::*)>::set)
 {
  _ASSERT_FIELD_JNI_COMPLIANCE
 }

 template<typename T>
 JFieldID::JFieldID(T * const staticMember, const char * const name, const uint32_t modifiers) noexcept :
  _jfieldID(),
  isStatic(true),
  modifiers(modifiers),
  staticProp((staticProp_t)staticMember),
  name(name),
  signature(_CX::JniTypeBase<T>::signature),
  proxyGetFunc((void (*)())&_CX::FieldAccessor<T*>::get),
  proxySetFunc((void (*)())&_CX::FieldAccessor<T*>::set)
 {
  _ASSERT_FIELD_JNI_COMPLIANCE
 }

 template<typename T>
 T JFieldID::get(JObject * const obj) {
  if (isStatic) {
   return ((T (*)(void *))proxyGetFunc)(staticProp);
  } else {
   return ((T (*)(void *, void *))proxyGetFunc)(obj, memberProp);
  }
 }

 //JMethodID template members
 template<typename R, typename... Args>
 JMethodID::JMethodID(R (* const func)(Args...), const uint32_t modifiers) noexcept :
  _jmethodID(),
  JNINativeMethod {
   const_cast<char *>("<init>"),
   verifySignature(_CX::SignatureGenerator<true, R, Args...>::signature),
   nullptr
  },
  type(STATIC_FUNC),
  modifiers(modifiers),
  staticFunc((static_func_t)func),
  proxyFuncV((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeV<>),
  proxyFuncA((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeA<>)
 {
  _ASSERT_JNI_FUNCTION_COMPLIANCE
 }

 template<typename R, typename... Args>
 JMethodID::JMethodID(R (* const func)(Args...), const char * const name, const uint32_t modifiers) noexcept :
  _jmethodID(),
  JNINativeMethod {
   verifyName(name),
   verifySignature(_CX::SignatureGenerator<false, R, Args...>::signature),
   nullptr
  },
  type(STATIC_FUNC),
  modifiers(modifiers),
  staticFunc((static_func_t)func),
  proxyFuncV((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeV<>),
  proxyFuncA((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeA<>)
 {
  _ASSERT_JNI_FUNCTION_COMPLIANCE
 }

 template<typename T, typename R, typename... Args>
 JMethodID::JMethodID(R (T::* const func)(Args...), const char * const name, const uint32_t modifiers) noexcept :
  _jmethodID(),
  //JNINativeMethod does not store const pointers to anything
  //const_cast is safe since we inherit privately from JNINativeMethod
  //and there is no risk of overwriting immutable pointers
  JNINativeMethod {
   verifyName(name),
   verifySignature(_CX::SignatureGenerator<false, R, Args...>::signature),
   nullptr
  },
  type(MEMBER_FUNC),
  modifiers(modifiers),
  memberFunc((member_func_t)func),
  proxyFuncV((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeV<>),
  proxyFuncA((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeA<>)
 {
  _ASSERT_JNI_FUNCTION_COMPLIANCE
 }

 //Performs virtual dispatch
 template<typename R, typename A>
 R JMethodID::invoke(JavaVM * const vm, void * const clazzOrInst, A args) const {
  auto* clazz = &((JObject *)clazzOrInst)->getClass();
  if (strcmp(clazz->getName(), "java/lang/Class") == 0) {
   //Static method, no virtual dispatch
   if (type == MEMBER_FUNC) {
    throw std::runtime_error(
     "FATAL: Tried to invoke '"
      + std::string(name)
      + signature
      + "' as a static method!"
    );
   }
   return internalInvoke<R, A>(vm, clazzOrInst, args);
  } else {
   //Member method, virtual dispatch
   while (clazz != JObject::getDescriptor()) {
    const auto& methods = clazz->getMethods();
    for (unsigned int i = 0; i < methods.getSize(); i++) {
     const auto method = methods[i];
     if (strcmp(name, method->getName()) == 0) {
      if (strcmp(signature, method->getSignature()) == 0) {
       return method->internalInvoke<R, A>(vm, clazzOrInst, args);
      }
     }
    }
    clazz = &clazz->parent;
   }
   throw std::runtime_error(
    "FATAL: Could not perform virtual function invocation for '"
    + std::string(name)
    + signature
    + "' since no classes in the inheritance hierarchy of '"
    + clazz->getName()
    + "'register a matching overload!"
   );
  }
 }

 //Does not perform virtual dispatch
 template<typename R, typename A>
 R JMethodID::invoke(JavaVM * const vm, JClass * const clazz, void * const inst, A args) const {
  const auto mid = clazz->getMethod(signature, name);
  if (!mid) {
   throw std::runtime_error(
    "FATAL: Class '"
     + std::string(clazz->getName())
     + "' does not contain a method matching the signature: '"
     + name
     + signature
     + "'!");
  }
  return mid->internalInvoke<R, A>(vm, inst, args);
 }

 template<typename R, typename A>
 R JMethodID::internalInvoke(JavaVM * vm, void * clazzOrInst, A args) const {
  using arg_t = typename CX::ComponentTypeResolver<A>::type;
  switch (type) {
   case MEMBER_FUNC:
    return ((R (*)(void * const, member_func_t, A))getFunctionProxy<A>())(clazzOrInst, memberFunc, args);
   case STATIC_FUNC:
    return ((R (*)(static_func_t, A))getFunctionProxy<A>())(staticFunc, args);
   case REGISTER_NATIVES_FUNC: {
    const auto argc = descriptor->nargs - 2;
    void * values[descriptor->nargs];
    values[0] = &((Jvm *)vm)->getJniEnv();
    values[1] = clazzOrInst;
    //set up arguments
    const auto resolverOffset = CX::IsSame<arg_t, jvalue>::value ? argc : 0;
    for (unsigned int i = 0; i < argc; i++) {
     values[i + 2] = ((void * (*)(A))resolvers[i + resolverOffset])(args);
    }
    //Would be CX::IsSame<T, va_list> but this comparison is always false
    if constexpr (!CX::IsSame<arg_t, jvalue>::value) {
     va_end(args);
    }
    if constexpr(CX::IsSame<R, void>::value) {
     ffi_call(descriptor, FFI_FN(fnPtr), nullptr, values);
     //clean up arguments
     for (unsigned int i = 0; i < argc; i++) {
      ((void (*)(void *))deallocators[i])(values[i + 2]);
     }
    } else {
     R r;
     ffi_call(descriptor, FFI_FN(fnPtr), (void *)&r, values);
     //clean up arguments
     for (unsigned int i = 0; i < argc; i++) {
      ((void (*)(void *))deallocators[i])(values[i + 2]);
     }
     return r;
    }
   }
  }
 }

 //Jvm template members
 template<typename T>
 bool Jvm::registerClass() {
  static_assert(
   __is_base_of(JObject, T),
   "Only native classes may be passed to registerClass!"
  );
  return registerClass(&T::descriptor);
 }

 template<typename T>
 bool Jvm::unregisterClass() {
  static_assert(
   __is_base_of(JObject, T),
   "Only native classes may be passed to unregisterClass!"
  );
  return unregisterClass(&T::descriptor);
 }

 //_CX::JClassImpl template members
 namespace _CX {
  template<typename T>
  constexpr JClassBreeder<T, nullptr>::JClassBreeder(decltype(descriptorElements) descriptorElements) noexcept :
   descriptorElements(descriptorElements)
  {
   JClassBreeder<T>::assertCompliance();
  }

  //Class members
  //TODO convert to template function breeder rather than lambda predicate
  template<typename T>
  template<typename A>
  constexpr typename JClassBreeder<T, nullptr>::template constructor_func_t<A> JClassBreeder<T, true>::constructorPredicate() noexcept {
   return [](JavaVM * const vm, const char * const signature, A args) -> JObject * {
    JClass& descriptor = const_cast<JClass&>(T::descriptor);
    Jvm * const jvm = (Jvm * const)vm;
    for (uint32_t i = 0; i < descriptor.functions.getSize(); i++) {
     JMethodID * const method = (descriptor.functions)[i];
     if (strcmp(method->getSignature(), signature) == 0 && strcmp(method->getName(), "<init>") == 0) {
      const auto inst = method->invoke<T *>(vm, nullptr, args);
      JObject * baseInst;
      if constexpr(_CX::JniTypeBase<T>::hasComplexHierarchy) {
       baseInst = (JObject *)T::cast::cast(inst);
      } else {
       baseInst = (JObject *)inst;
      }
//      return const_cast<AllocStack<JObject *>&>(jvm->getInstances()).pushAlloc(JClassBreeder<T>::deallocator, baseInst);
      return (*jvm)[&descriptor].pushAlloc(&_CX::Deallocator<T>::deallocate, baseInst);
     }
    }
    return nullptr;
   };
  }

  template<typename T>
  constexpr void JClassBreeder<T, true>::assertCompliance() noexcept {
   static_assert(__is_base_of(JObject, T), "T must be derived from JObject!");
   //Perform checks for complex hierarchy 'cast' alias member
   //TODO if cast is not defined use default casting route
   CastDefined<T>::assertAliasCorrectness();
   //TODO waiting on constexpr string utils from CX
   //TODO ensure that the names dont contain [, L or ;
//   const auto name = T::name;
  }

  //Non-class members
  template<typename T>
  template<typename A>
  constexpr typename JClassBreeder<T, nullptr>::template constructor_func_t<A> JClassBreeder<T, false>::constructorPredicate() noexcept {
   return [](JavaVM * const vm, const char * const signature, A args) -> JObject * {
    throw std::runtime_error("FATAL: You cannot construct primitive types!");
   };
  }

  template<typename T>
  constexpr void JClassBreeder<T, false>::assertCompliance() noexcept {
   //TODO ensure that the names dont contain [, L or ;
  }
 }

 //JValueArgResolver for JObject derived classes (see meta/method.h)
 namespace _CX {
  template<typename T>
  class JValueArgResolver<true, T> {

  public:
   inline static constexpr const bool isRegisteredResolver = true;
   using componentType = typename ComponentTypeResolver<T*>::type;

   [[gnu::always_inline]]
   inline static componentType* getAArg(jvalue *values) {
    static_assert(__is_base_of(_jobject, componentType), "Illegal JNI function parameter type!");
    if constexpr(CastDefined<componentType>::value) {
     return componentType::cast::cast((JObject*)values->l);
    } else {
     return (componentType *)values->l;
    }
   }
  };
 }

 //JClass template members
 template<typename T>
 JClass::JClass(uint32_t modifiers, _CX::JClassBreeder<T, true> breeder) noexcept :
  JObject(),
  constructV(decltype(breeder)::template constructorPredicate<va_list>()),
  constructA(decltype(breeder)::template constructorPredicate<const jvalue *>()),
  isArbitrary(false),
  className(T::name),
  modifiers(modifiers),
  parent(breeder.parent),
  functions{true},
  fields{true}
 {
  for (const auto& d : breeder.descriptorElements) {
   d.process(this);
  }
 }

 template<typename T>
 JClass::JClass(uint32_t modifiers, _CX::JClassBreeder<T, false> breeder) noexcept :
  JObject(),
  constructV(decltype(breeder)::template constructorPredicate<va_list>()),
  constructA(decltype(breeder)::template constructorPredicate<const jvalue *>()),
  isArbitrary(false),
  className(_CX::JniTypeBase<T>::signature),
  modifiers(modifiers),
  parent(breeder.parent),
  functions{true},
  fields{true}
 {
  for (const auto& d : breeder.descriptorElements) {
   d.process(this);
  }
 }

//Primitive type class descriptors
 extern const JClass
  voidDescriptor,
  booleanDescriptor,
  byteDescriptor,
  shortDescriptor,
  intDescriptor,
  floatDescriptor,
  longDescriptor,
  doubleDescriptor;
}