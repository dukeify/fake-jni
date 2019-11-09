#pragma once

#include "fake-jni/internal/util.h"
#include "fake-jni/internal/meta/types.h"
#include "fake-jni/internal/meta/field.h"
#include "fake-jni/internal/meta/method.h"

#include <cx/templates.h>
#include <cx/indirection.h>
#include <cx/classes.h>
#include <cx/idioms.h>
#include <cx/strings.h>
#include <cx/unsafe.h>
#include <cx/vararg.h>
#include <cx/lambda.h>

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
#include <utility>

//Internal JFieldID macros
#define _ASSERT_FIELD_JNI_COMPLIANCE \
static_assert(\
 __is_base_of(_jobject, T) || _CX::JniTypeBase<T>::isRegisteredType,\
 "Field type is not a valid JNI type!"\
);\
static_assert(\
 CX::ComponentTypeResolver<T>::indirectionCount == 0U,\
 "Registered field types cannot be pointers or references to a JNI type!"\
);

#define _JFIELDID_STATIC_CHECK(expr) \
if ((modifiers & STATIC) == STATIC) {\
 if (fid == this) {\
  expr\
 }\
} else {\
 expr\
}\
break;

//Internal JMethodID macros
#define _ASSERT_JNI_FUNCTION_COMPLIANCE \
static_assert(\
 _CX::VerifyJniFunctionArguments<R>::verify(),\
 "Registered JNI functions may only return JNI types and pointers to JObject or derived classes!"\
);\
static_assert(\
 _CX::VerifyJniFunctionArguments<Args...>::verify(),\
 "Registered JNI functions may only accept JNI types and pointers to JObject or derived classes!"\
);

#define _INTERNAL_INVOKE_VA_ARG(type, ffi_type) \
if (ffi_arg_t == &ffi_type) {\
 auto *value = values[i + 2];\
 *((type *)value) = CX::safe_va_arg<type>(args);\
 continue;\
}

#define _INTERNAL_INVOKE_CLEANUP \
delete (JNIEnv **)values[0];\
delete (jobject *)values[1];\
for (unsigned int i = 0; i < argc; i++) {\
 ((void (*)(void *))deallocators[i])(values[i + 2]);\
}

//FAKE-JNI API MACROS
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

//Must be defined before metadata templates are instantiated
DEFINE_JNI_TYPE(FakeJni::JObject, "Ljava/lang/Object;")

//fake-jni API declarations
namespace FakeJni {
 class Jvm;

//JNI / JVMTI WRAPPER API
 class InvokeInterface : public JNIInvokeInterface_ {
 public:
  Jvm& vm;

  //jni/invoke/invoke.cpp
  explicit InvokeInterface(const Jvm& vm);
  explicit InvokeInterface(const InvokeInterface&) = delete;
  virtual ~InvokeInterface() = default;
  InvokeInterface& operator=(const InvokeInterface& ii) = delete;

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
  Jvm& vm;

  //jni/native/native.cpp
  explicit NativeInterface(const Jvm& vm);
  explicit NativeInterface(const NativeInterface &) = delete;
  virtual ~NativeInterface() = default;
//  virtual NativeInterface& operator=(const NativeInterface& ni) noexcept;
  NativeInterface& operator=(const NativeInterface& ni) = delete;

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
  virtual jobject newObjectV(jclass, jmethodID, CX::va_list_t&) const;
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
  virtual jobject callObjectMethodV(jobject, jmethodID, CX::va_list_t&) const;
  virtual jobject callObjectMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jboolean callBooleanMethod(jobject, jmethodID, ...) const;
  virtual jboolean callBooleanMethodV(jobject, jmethodID, CX::va_list_t&) const;
  virtual jboolean callBooleanMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jbyte callByteMethod(jobject, jmethodID, ...) const;
  virtual jbyte callByteMethodV(jobject, jmethodID, CX::va_list_t&) const;
  virtual jbyte callByteMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jchar callCharMethod(jobject, jmethodID, ...) const;
  virtual jchar callCharMethodV(jobject, jmethodID, CX::va_list_t&) const;
  virtual jchar callCharMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jshort callShortMethod(jobject, jmethodID, ...) const;
  virtual jshort callShortMethodV(jobject, jmethodID, CX::va_list_t&) const;
  virtual jshort callShortMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jint callIntMethod(jobject, jmethodID, ...) const;
  virtual jint callIntMethodV(jobject, jmethodID, CX::va_list_t&) const;
  virtual jint callIntMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jlong callLongMethod(jobject, jmethodID, ...) const;
  virtual jlong callLongMethodV(jobject, jmethodID, CX::va_list_t&) const;
  virtual jlong callLongMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jfloat callFloatMethod(jobject, jmethodID, ...) const;
  virtual jfloat callFloatMethodV(jobject, jmethodID, CX::va_list_t&) const;
  virtual jfloat callFloatMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jdouble callDoubleMethod(jobject, jmethodID, ...) const;
  virtual jdouble callDoubleMethodV(jobject, jmethodID, CX::va_list_t&) const;
  virtual jdouble callDoubleMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual void callVoidMethod(jobject, jmethodID, ...) const;
  virtual void callVoidMethodV(jobject, jmethodID, CX::va_list_t&) const;
  virtual void callVoidMethodA(jobject, jmethodID, const jvalue *) const;
//  virtual jobject callNonvirtualObjectMethod(jobject, jclass, jmethodID, ...) const;
  virtual jobject callNonvirtualObjectMethodV(jobject, jclass, jmethodID, CX::va_list_t&) const;
  virtual jobject callNonvirtualObjectMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jboolean callNonvirtualBooleanMethod(jobject, jclass, jmethodID, ...) const;
  virtual jboolean callNonvirtualBooleanMethodV(jobject, jclass, jmethodID, CX::va_list_t&) const;
  virtual jboolean callNonvirtualBooleanMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jbyte callNonvirtualByteMethod(jobject, jclass, jmethodID, ...) const;
  virtual jbyte callNonvirtualByteMethodV(jobject, jclass, jmethodID, CX::va_list_t&) const;
  virtual jbyte callNonvirtualByteMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jchar callNonvirtualCharMethod(jobject, jclass, jmethodID, ...) const;
  virtual jchar callNonvirtualCharMethodV(jobject, jclass, jmethodID, CX::va_list_t&) const;
  virtual jchar callNonvirtualCharMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jshort callNonvirtualShortMethod(jobject, jclass, jmethodID, ...) const;
  virtual jshort callNonvirtualShortMethodV(jobject, jclass, jmethodID, CX::va_list_t&) const;
  virtual jshort callNonvirtualShortMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jint callNonvirtualIntMethod(jobject, jclass, jmethodID, ...) const;
  virtual jint callNonvirtualIntMethodV(jobject, jclass, jmethodID, CX::va_list_t&) const;
  virtual jint callNonvirtualIntMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jlong callNonvirtualLongMethod(jobject, jclass, jmethodID, ...) const;
  virtual jlong callNonvirtualLongMethodV(jobject, jclass, jmethodID, CX::va_list_t&) const;
  virtual jlong callNonvirtualLongMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jfloat callNonvirtualFloatMethod(jobject, jclass, jmethodID, ...) const;
  virtual jfloat callNonvirtualFloatMethodV(jobject, jclass, jmethodID, CX::va_list_t&) const;
  virtual jfloat callNonvirtualFloatMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual jdouble callNonvirtualDoubleMethod(jobject, jclass, jmethodID, ...) const;
  virtual jdouble callNonvirtualDoubleMethodV(jobject, jclass, jmethodID, CX::va_list_t&) const;
  virtual jdouble callNonvirtualDoubleMethodA(jobject, jclass, jmethodID, const jvalue *) const;
//  virtual void callNonvirtualVoidMethod(jobject, jclass, jmethodID, ...) const;
  virtual void callNonvirtualVoidMethodV(jobject, jclass, jmethodID, CX::va_list_t&) const;
  virtual void callNonvirtualVoidMethodA(jobject, jclass, jmethodID, const jvalue *) const;
  virtual jmethodID getStaticMethodID(jclass, const char *, const char *) const;
//  virtual jobject callStaticObjectMethod(jclass, jmethodID, ...) const;
  virtual jobject callStaticObjectMethodV(jclass, jmethodID, CX::va_list_t&) const;
  virtual jobject callStaticObjectMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jboolean callStaticBooleanMethod(jclass, jmethodID, ...) const;
  virtual jboolean callStaticBooleanMethodV(jclass, jmethodID, CX::va_list_t&) const;
  virtual jboolean callStaticBooleanMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jbyte callStaticByteMethod(jclass, jmethodID, ...) const;
  virtual jbyte callStaticByteMethodV(jclass, jmethodID, CX::va_list_t&) const;
  virtual jbyte callStaticByteMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jchar callStaticCharMethod(jclass, jmethodID, ...) const;
  virtual jchar callStaticCharMethodV(jclass, jmethodID, CX::va_list_t&) const;
  virtual jchar callStaticCharMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jshort callStaticShortMethod(jclass, jmethodID, ...) const;
  virtual jshort callStaticShortMethodV(jclass, jmethodID, CX::va_list_t&) const;
  virtual jshort callStaticShortMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jint callStaticIntMethod(jclass, jmethodID, ...) const;
  virtual jint callStaticIntMethodV(jclass, jmethodID, CX::va_list_t&) const;
  virtual jint callStaticIntMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jlong callStaticLongMethod(jclass, jmethodID, ...) const;
  virtual jlong callStaticLongMethodV(jclass, jmethodID, CX::va_list_t&) const;
  virtual jlong callStaticLongMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jfloat callStaticFloatMethod(jclass, jmethodID, ...) const;
  virtual jfloat callStaticFloatMethodV(jclass, jmethodID, CX::va_list_t&) const;
  virtual jfloat callStaticFloatMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual jdouble callStaticDoubleMethod(jclass, jmethodID, ...) const;
  virtual jdouble callStaticDoubleMethodV(jclass, jmethodID, CX::va_list_t&) const;
  virtual jdouble callStaticDoubleMethodA(jclass, jmethodID, const jvalue *) const;
//  virtual void callStaticVoidMethod(jclass, jmethodID, ...) const;
  virtual void callStaticVoidMethodV(jclass, jmethodID, CX::va_list_t&) const;
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
  [[nodiscard]]
  virtual jbooleanArray newBooleanArray(jsize) const;
  [[nodiscard]]
  virtual jbyteArray newByteArray(jsize) const;
  [[nodiscard]]
  virtual jcharArray newCharArray(jsize) const;
  [[nodiscard]]
  virtual jshortArray newShortArray(jsize) const;
  [[nodiscard]]
  virtual jintArray newIntArray(jsize) const;
  [[nodiscard]]
  virtual jlongArray newLongArray(jsize) const;
  [[nodiscard]]
  virtual jfloatArray newFloatArray(jsize) const;
  [[nodiscard]]
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
  Jvm& vm;

  //jvmti/jvmti.cpp
  explicit JvmtiInterface(const Jvm& vm);
  explicit JvmtiInterface(const JvmtiInterface&) = delete;
  virtual ~JvmtiInterface() = default;
  JvmtiInterface& operator=(const JvmtiInterface& ji) = delete;

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
  Jvm& vm;

 public:
  //jni/jni_env.cpp
  explicit JniEnv(const Jvm& vm) noexcept;
  explicit JniEnv(const JniEnv&) = delete;
  virtual ~JniEnv() = default;
  JniEnv& operator=(const JniEnv& env) = delete;
  virtual const Jvm& getVM() const noexcept;
  virtual void setNativeInterface(NativeInterface& interface) noexcept;
 };

 class JvmtiEnv : public jvmtiEnv {
 public:
  Jvm& vm;

  explicit JvmtiEnv(const Jvm& vm) noexcept;
  explicit JvmtiEnv(const JvmtiEnv&) = delete;
  virtual ~JvmtiEnv() = default;
  JvmtiEnv& operator=(const JvmtiEnv& env) = delete;
  virtual const Jvm& getVM() const noexcept;
  void setJvmtiInterface(JvmtiInterface& interface) noexcept;
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

 //Jni java/lang/Object implementation
 class JObject {
 public:
  //Internal fake-jni native class metadata
  //DEFINE_CLASS_NAME cant be used since this is the virtual base
  static constexpr const char name[] = "java/lang/Object";
  static const JClass descriptor;

  inline static const JClass * getDescriptor() noexcept {
   return &descriptor;
  }

  template<typename T>
  operator T() const;

  JObject() = default;
  virtual ~JObject() = default;
  virtual const JClass & getClass() const noexcept;
 };

 //fake-jni implementation
 class JFieldID final : public _jfieldID {
 public:
  template<typename T>
  using get_func_t = T& (*)(void * inst);
  template<typename T>
  using set_func_t = void (*)(void * inst, T * value);
  using v_get_func_t = void * (*)(void * inst);
  using v_set_func_t = void (*)(void * inst, void * value);

  using static_prop_t = void * const;
  using member_prop_t = int (_CX::AnyClass::* const);

  enum Type {
   STATIC_PROP,
   MEMBER_PROP,
   CALLBACK_PROP,
   STL_CALLBACK_PROP
  };

  const Type type;

 private:
  const uint32_t modifiers;
  const char
   * const name,
   * const signature;

  union {
   //TODO CALLBACK_PROP only needs the proxy functions
   //STATIC_PROP, MEMBER_PROP and CALLBACK_PROP
   struct {
    union {
     static_prop_t staticProp;
     member_prop_t memberProp;
    };
    void
     (* const proxyGetFunc)(),
     (* const proxySetFunc)();
   };
   //STL_CALLBACK_PROP
   struct {
    const _CX::arbitrary_align_t<sizeof(CX::Lambda<void ()>)>
     arbitraryGet,
     arbitrarySet;
   };
  };

  const JFieldID * findVirtualMatch(const JClass * clazz) const noexcept;

 public:
  const bool isArbitrary;

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
  //Constructor for arbitrary fields with user-defined access callbacks
  template<typename T>
  JFieldID(get_func_t<T> get, set_func_t<T> set, const char * name, uint32_t modifiers) noexcept;
  //Constructor for arbitrary fields with type-erased user-defined access callbacks
  JFieldID(v_get_func_t get, v_set_func_t set, const char * name, const char * signature, uint32_t modifiers) noexcept;
  //Constructor for arbitrary fields with user-defined, capturing, access callbacks
  template<typename T>
  JFieldID(CX::Lambda<T* ()> get, CX::Lambda<void (T*)> set, const char * name, uint32_t modifiers) noexcept;
  //Constructor for arbitrary fields with type-erased user-defined, capturing, access callbacks
  JFieldID(CX::Lambda<void * ()> get, CX::Lambda<void (void *)> set, const char * name, const char * signature, uint32_t modifiers) noexcept;

  inline const char * getName() const noexcept {
   return name;
  }

  inline const char * getSignature() const noexcept {
   return signature;
  }

  inline uint32_t getModifiers() const noexcept {
   return modifiers;
  }

  bool operator ==(const JFieldID &fid) const noexcept;
  template<typename T>
  T& get(JObject * obj) const;
  void set(JObject * obj, void * value) const;
 };

 //fake-jni implementation
 //TODO protect the mutable fields in JNINativeMethod
// class JMethodID final : public _jmethodID, private JNINativeMethod {
 class JMethodID final : public _jmethodID, public JNINativeMethod {
 public:
  using static_func_t = void (*)();
  using member_func_t = void (_CX::AnyClass::*)();
  using arbitrary_func_t = void * (*)(JNIEnv * env, jobject objOrInst, ...);
  using void_func_t = void (*)();

  enum Type {
   STATIC_FUNC,
   MEMBER_FUNC,
   REGISTER_NATIVES_FUNC,
   STL_FUNC,
   ARBITRARY_STL_FUNC
  };

  const Type type;

 private:
  //Ref-counted map of signature hashes to ffi_cif descriptors
  static std::map<size_t, std::pair<unsigned long, ffi_cif *>> descriptors;

  union {
   //STATIC_FUNC, MEMBER_FUNC and STL_FUNC
   struct {
    //Functions registered through registerNatives do not have modifiers
    const uint32_t modifiers;
    union {
     //STATIC_FUNC and MEMBER_FUNC
     //high bytes of member pointer for vtable offset in `this`
     void * adj;
     //STL_FUNC
     _CX::arbitrary_align_t<sizeof(CX::Lambda<void ()>) - sizeof(fnPtr)> stlFunc;
    };
    static_func_t
     proxyFuncV,
     proxyFuncA;
   };
   //REGISTER_NATIVES_FUNC
   struct {
    ffi_cif * descriptor;
    void_func_t
     * const resolvers,
     * const deallocators;
   };
  };

  template<typename T>
  [[gnu::always_inline nodiscard]]
  static_func_t getFunctionProxy() const {
   using component_t = typename CX::ComponentTypeResolver<T>::type;
   if (type == REGISTER_NATIVES_FUNC) {
    throw std::runtime_error(
     "FATAL: Internal error: JMethodID::getFunctionProxy<T> only accepts CX::va_list_t and jvalue as type parameters!"
    );
   } else {
    if constexpr(CX::IsSame<component_t, CX::va_list_t>::value) {
     return proxyFuncV;
    } else if constexpr(CX::IsSame<component_t, jvalue>::value) {
     return proxyFuncA;
    }
   }
  }

  static /*const*/ char * verifyName(const char * name);
  static /*const*/ char * verifySignature(const char * sig);
  [[nodiscard]]
  static std::vector<ffi_type *> getFfiPrototype(const char * signature, const char * name);

  template<typename R, typename A>
  R internalInvoke(const JavaVM * vm, void * clazzOrInst, A& args) const;

 public:
  const bool isArbitrary;

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
  JMethodID(R (* func)(Args...), uint32_t modifiers) noexcept;
  //constructor for static functions
  template<typename R, typename... Args>
  JMethodID(R (* func)(Args...), const char * name, uint32_t modifiers) noexcept;
  //Constructor for member methods
  template<typename T, typename R, typename... Args>
  JMethodID(R (T::* func)(Args...), const char * name, uint32_t modifiers) noexcept;
  //Constructor for const member methods
  template<typename T, typename R, typename... Args>
  JMethodID(R (T::* func)(Args...) const, const char * name, uint32_t modifiers) noexcept;
  //Constructor for RegisterNatives
  JMethodID(const JNINativeMethod * method);
  //Constructor for capturing lambdas (non-capturing lambdas are implicitly converted to static functions)
  template<typename R, typename... Args>
  JMethodID(CX::Lambda<R (Args...)> func, const char * name, uint32_t modifiers) noexcept;
  //Constructor for arbitrary functions and non-capturing lambdas
  JMethodID(arbitrary_func_t func, const char * signature, const char * name, uint32_t modifiers);
  //Constructor for arbitrary capturing lambdas
  //TODO once signature checking is in place, add support for this
//  template<typename... Args>
//  JMethodID(CX::Lambda<void * (JNIEnv *, jobject, Args...)> func, const char * signature, const char * name, uint32_t modifiers);
  JMethodID(CX::Lambda<void * (JNIEnv *, jobject, jvalue *)> func, const char * signature, const char * name, uint32_t modifiers);
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
  R virtualInvoke(const JavaVM *vm, void *clazzOrInst, A& args) const;
  template<typename R, typename A>
  R nonVirtualInvoke(const JavaVM *vm, JClass *const clazz, void *const inst, A& args) const;
  template<typename R>
  R invoke(const JavaVM * vm, const JObject * clazzOrInst, ...) const;
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
   using constructor_func_t = JObject * (*)(const JavaVM * vm, const char * signature, A args);

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
   static JObject * constructorPredicate(const JavaVM * vm, const char * signature, A args);

   static constexpr void assertCompliance() noexcept;
  };

  template<typename T>
  struct JClassBreeder<T, false> final : JClassBreeder<T, nullptr> {
   using JClassBreeder<T, nullptr>::JClassBreeder;

   template<typename A>
   static JObject * constructorPredicate(const JavaVM * vm, const char * signature, A args);

   static constexpr void assertCompliance() noexcept;
  };
 }

 class JClass : public JObject {
 private:
  JObject
   * (* const constructV)(const JavaVM *, const char *, CX::va_list_t&),
   * (* const constructA)(const JavaVM *, const char *, const jvalue *);

  const char * const className;

 public:
  DEFINE_CLASS_NAME("java/lang/Class")
  //Internal fake-jni native class metadata

  const bool isArbitrary;

  template<typename T>
  operator T() const;

  const uint32_t modifiers;
  const JClass& parent;

  //Property associations
  PointerList<const JMethodID *> functions;
  PointerList<const JFieldID *> fields;

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

  template<typename T>
  explicit JClass(uint32_t modifiers, _CX::JClassBreeder<T, true> breeder) noexcept;
  template<typename T>
  explicit JClass(uint32_t modifiers, _CX::JClassBreeder<T, false> breeder) noexcept;
  explicit JClass(const char * name, uint32_t modifiers = PUBLIC) noexcept;
  JClass(const JClass& clazz) noexcept;
  virtual ~JClass() = default;

  uint32_t getModifiers() const noexcept;
  bool registerMethod(const JMethodID * mid, bool deallocate = true) const;
  bool unregisterMethod(const JMethodID * mid) const noexcept;
  const JMethodID * getMethod(const char * sig, const char * name) const noexcept;
  [[nodiscard]]
  const PointerList<const JMethodID *>& getMethods() const noexcept;
  bool registerField(JFieldID * fid, bool deallocate = true) const noexcept;
  bool unregisterField(JFieldID * fid) const noexcept;
  const JFieldID * getField(const char * name) const noexcept;
  const JFieldID * getField(const char * sig, const char * name) const noexcept;
  [[nodiscard]]
  const PointerList<const JFieldID *>& getFields() const noexcept;
  virtual const char * getName() const noexcept;
  //Object construction for c-varargs
  [[nodiscard]]
  JObject * newInstance(const JavaVM * vm, const char * signature, CX::va_list_t& list) const;
  //Object construction for jvalue arrays
  [[nodiscard]]
  JObject * newInstance(const JavaVM * vm, const char * signature, const jvalue * values) const;
 };

 //FAKE-JNI USER API
 //TODO all JVM calls should be blocking until execution completes to prevent race conditions from emerging
 class Jvm : public JavaVM {
 public:
  const char * const uuid;

 private:
  jthrowable exception = nullptr;
  FILE * const log;
  InvokeInterface * invoke;
  NativeInterface * native;
  JvmtiInterface * jvmti;
  JniEnv * jniEnv;
  JvmtiEnv * jvmtiEnv;

  //TODO on the first invocation of any JNI, JNIEnv or JVMTI functions, set this flag to true
  bool running;

  PointerList<const Library *> libraries;
  PointerList<const JClass *> classes;
  //TODO if classloaders are ever implemented, this property will be handled by the ClassLoader model
  std::map<const JClass *, PointerList<JObject *>> instances;
  mutable std::shared_mutex instances_mutex;

  bool removeLibrary(const Library * library, const std::string & options);

 protected:
  static PointerList<Jvm *> vms;
  static thread_local const Jvm * currentVm;

 public:
  struct UnwindException final : std::runtime_error {
   using std::runtime_error::runtime_error;
  };

  explicit Jvm(FILE * log = stdout);
  virtual ~Jvm();

  Jvm& operator=(const Jvm&) const = delete;

  //Other properties
  virtual FILE * getLog() const;
  virtual void registerDefaultClasses();
  virtual void registerDefaultSignalHandler();
  virtual const char * generateJvmUuid() noexcept;
  static const Jvm * getCurrentVm() noexcept;

  //Interface and Env api
  template<typename T>
  void setInvokeInterface();
  virtual void setInvokeInterface(InvokeInterface * ii);
  virtual InvokeInterface& getInvokeInterface() const;

  template<typename T>
  void setNativeInterface();
  virtual void setNativeInterface(NativeInterface * ni);
  virtual NativeInterface& getNativeInterface() const;

  template<typename T>
  void setJvmtiInterface();
  virtual void setJvmtiInterface(JvmtiInterface * jvmti);
  virtual JvmtiInterface& getJvmtiInterface() const;

  template<typename T>
  void setJniEnv();
  virtual void setJniEnv(JniEnv * env);
  virtual JniEnv& getJniEnv() const;

  template<typename T>
  void setJvmtiEnv();
  virtual void setJvmtiEnv(JvmtiEnv * env);
  virtual JvmtiEnv& getJvmtiEnv() const;

  virtual const PointerList<const JClass *>& getClasses() const;
  virtual const PointerList<JObject *>& operator[](const JClass * clazz) const;
  virtual PointerList<JObject *>& operator[](const JClass * clazz);
  virtual const decltype(instances)& getAllInstances() const;
  virtual bool addInstance(JObject * inst);
  virtual bool removeInstance(JObject * inst);
  virtual bool isRunning() const;

  template<typename T>
  bool registerClass();
  template<typename T>
  bool unregisterClass();
  virtual bool registerClass(const JClass * clazz, bool deallocate = false);
  virtual bool unregisterClass(const JClass * clazz);
  virtual const JClass * findClass(const char * name) const;
  //Needs to be const to allow attaching agents / JNI modules at runtime
  virtual void attachLibrary(
   const std::string & rpath,
   const std::string & options = "",
   LibraryOptions loptions = {&dlopen, &dlsym, &dlclose}
  );
  virtual bool removeLibrary(const std::string & path, const std::string & options = "");
  virtual const PointerList<const Library *>& getLibraries() const;
  virtual void start();
  virtual JInt destroy();
  virtual void throwException(jthrowable throwable);
  virtual jthrowable getException() const;
  virtual void clearException();
  //Does not return
  [[noreturn]]
  virtual void fatalError(const char * message) const;
  virtual void printBacktrace() const;
 };

 //Template glue code for native class registration
 namespace _CX {
  class ClassDescriptorElement {
  public:
   const std::function<bool (JClass * const)> processHook;
   //TODO FunctionOperatorExists static assertion fails?
//   const CX::Lambda<bool (JClass * const)> processHook;

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

  template<typename T>
  class VerifyJniFunctionArguments<T> {
  public:
   [[gnu::always_inline]]
   inline static constexpr bool verify() {
    using resolver = ComponentTypeResolver<T>;
    using arg_t = typename resolver::type;
    if constexpr(__is_class(arg_t)) {
     return __is_base_of(JObject, arg_t) && resolver::indirectionCount == 1U;
    } else {
     return JniTypeBase<T>::isRegisteredType && resolver::indirectionCount == 0U;
    }
   }
  };
 }
}

//Create type metadata for JClass
DECLARE_NATIVE_TYPE(FakeJni::JClass)

//fake-jni API definitions
namespace FakeJni {
 //JObject template members
 template<typename T>
 JObject::operator T() const {
  using component_t = typename CX::ComponentTypeResolver<T>::type;
  constexpr const auto
   downcast = __is_base_of(JObject, T),
   jnicast = CX::IsSame<_jobject, component_t>::value;
  static_assert(
   downcast || jnicast,
   "JObject can only be downcasted and converted to _jobject!"
  );
  auto ptr = const_cast<JObject *>(this);
  if constexpr(downcast) {
   return (T&)*ptr;
  } else if constexpr(jnicast) {
   return CX::union_cast<jobject>(ptr);
  }
 }

 //JFieldID template members
 template<typename T, typename M>
 JFieldID::JFieldID(T M::* const member, const char * const name, const uint32_t modifiers) noexcept :
  _jfieldID(),
  type(MEMBER_PROP),
  modifiers(modifiers),
  name(name),
  signature(_CX::JniTypeBase<T>::signature),
  memberProp((member_prop_t)member),
  proxyGetFunc((void (*)())&_CX::FieldAccessor<T (M::*)>::get),
  proxySetFunc((void (*)())&_CX::FieldAccessor<T (M::*)>::set),
  isArbitrary(false)
 {
  _ASSERT_FIELD_JNI_COMPLIANCE
 }

 template<typename T>
 JFieldID::JFieldID(T * const staticMember, const char * const name, const uint32_t modifiers) noexcept :
  _jfieldID(),
  type(STATIC_PROP),
  modifiers(modifiers),
  name(name),
  signature(_CX::JniTypeBase<T>::signature),
  staticProp((static_prop_t)staticMember),
  proxyGetFunc((void (*)())&_CX::FieldAccessor<T*>::get),
  proxySetFunc((void (*)())&_CX::FieldAccessor<T*>::set),
  isArbitrary(false)
 {
  _ASSERT_FIELD_JNI_COMPLIANCE
 }

 template<typename T>
 JFieldID::JFieldID(get_func_t<T> get, set_func_t<T> set, const char * name, uint32_t modifiers) noexcept :
  _jfieldID(),
  type(CALLBACK_PROP),
  modifiers(modifiers),
  name(name),
  signature(_CX::JniTypeBase<T>::signature),
  proxyGetFunc((void (*)())get),
  proxySetFunc((void (*)())set),
  isArbitrary(true)
 {
  _ASSERT_FIELD_JNI_COMPLIANCE
 }

 template<typename T>
 JFieldID::JFieldID(CX::Lambda<T* ()> get, CX::Lambda<void (T*)> set, const char *name, uint32_t modifiers) noexcept :
  _jfieldID(),
  type(STL_CALLBACK_PROP),
  modifiers(modifiers),
  name(name),
  signature(_CX::JniTypeBase<T>::signature),
  arbitraryGet(std::move(CX::union_cast<decltype(arbitraryGet)>(std::move(get)))),
  arbitrarySet(std::move(CX::union_cast<decltype(arbitrarySet)>(std::move(set)))),
  isArbitrary(true)
 {
  _ASSERT_FIELD_JNI_COMPLIANCE
 }

 template<typename T>
 T& JFieldID::get(JObject * const obj) const {
  const auto& clazz = obj->getClass();
  auto * fid = findVirtualMatch(&clazz);
  if (fid) {
   switch (fid->type) {
    case STATIC_PROP: {
     _JFIELDID_STATIC_CHECK(
      return ((T& (*)(static_prop_t))fid->proxyGetFunc)(staticProp);
     )
    }
    case MEMBER_PROP: {
     _JFIELDID_STATIC_CHECK(
      return ((T& (*)(void *, member_prop_t))fid->proxyGetFunc)(obj, memberProp);
     )
    }
    case CALLBACK_PROP: {
     return ((T& (*)(void *))proxyGetFunc)(obj);
    }
    case STL_CALLBACK_PROP: {
     return *CX::union_cast<CX::Lambda<T* ()>>(arbitraryGet)();
    }
   }
  }
  throw std::runtime_error(
   "FATAL: Class '"
   + std::string(clazz.getName())
   + "' does not contain or inherit any fields matching '"
   + name
   + signature
   + "'!"
  );
 }

 //JMethodID template members
 template<typename R, typename... Args>
 JMethodID::JMethodID(R (* const func)(Args...), const uint32_t modifiers) noexcept :
  _jmethodID(),
  JNINativeMethod {
   const_cast<char *>("<init>"),
   verifySignature(_CX::SignatureGenerator<true, R, Args...>::signature),
   (void *)func
  },
  type(STATIC_FUNC),
  modifiers(modifiers),
  proxyFuncV((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeV<>),
  proxyFuncA((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeA<>),
  isArbitrary(false)
 {
  _ASSERT_JNI_FUNCTION_COMPLIANCE
 }

 template<typename T, typename R, typename... Args>
 JMethodID::JMethodID(R (T::* func)(Args...) const, const char * name, uint32_t modifiers) noexcept :
  JMethodID((R (T::*)(Args...))func, name, modifiers)
 {}

 template<typename R, typename... Args>
 JMethodID::JMethodID(R (* const func)(Args...), const char * const name, const uint32_t modifiers) noexcept :
  _jmethodID(),
  JNINativeMethod {
   verifyName(name),
   verifySignature(_CX::SignatureGenerator<false, R, Args...>::signature),
   (void *)func
  },
  type(STATIC_FUNC),
  modifiers(modifiers),
  proxyFuncV((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeV<>),
  proxyFuncA((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeA<>),
  isArbitrary(false)
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
   //low bytes of member pointer
   CX::union_cast<CX::member_ptr_align_t>(func).ptr
  },
  type(MEMBER_FUNC),
  modifiers(modifiers),
  //high bytes of member pointer
  adj(CX::union_cast<CX::member_ptr_align_t>(func).offset),
  proxyFuncV((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeV<>),
  proxyFuncA((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeA<>),
  isArbitrary(false)
 {
  _ASSERT_JNI_FUNCTION_COMPLIANCE
 }

 template<typename R, typename... Args>
 JMethodID::JMethodID(CX::Lambda<R (Args...)> func, const char * name, uint32_t modifiers) noexcept :
  JNINativeMethod {
   verifyName(name),
   verifySignature(_CX::SignatureGenerator<false, R, Args...>::signature),
   //segmented functor object
   CX::union_cast<decltype(fnPtr)>(func)
  },
  type(STL_FUNC),
  modifiers(modifiers),
  //store remaining functor object data
  stlFunc(CX::union_cast<decltype(stlFunc)>(*(CX::union_cast<char *>(&func) + sizeof(fnPtr)))),
  proxyFuncV((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeV<>),
  proxyFuncA((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeA<>),
  isArbitrary(false)
 {
  _ASSERT_JNI_FUNCTION_COMPLIANCE
 }

 //Performs virtual dispatch
 template<typename R, typename A>
 R JMethodID::virtualInvoke(const JavaVM * vm, void * clazzOrInst, A& args) const {
  auto * clazz = &((JObject *)clazzOrInst)->getClass();
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
   const auto * jobjDescriptor = JObject::getDescriptor();
   while (clazz != jobjDescriptor) {
    for (auto& method : clazz->getMethods()) {
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
 R JMethodID::nonVirtualInvoke(const JavaVM * vm, JClass * const clazz, void * const inst, A& args) const {
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

 template<typename R>
 R JMethodID::invoke(const JavaVM * const vm, const JObject * clazzOrInst, ...) const {
  CX::va_list_t list;
  va_start(list, clazzOrInst);
  return internalInvoke<R>(vm, (void *)clazzOrInst, list);
 }

 template<typename R, typename A>
 R JMethodID::internalInvoke(const JavaVM * vm, void * clazzOrInst, A& args) const {
  using arg_t = typename CX::ComponentTypeResolver<A>::type;
  //used to reassemble functor object data
  using align_t = _CX::arbitrary_align_t<sizeof(CX::Lambda<void ()>)>;
  struct Data {
   decltype(fnPtr) d1;
   decltype(stlFunc) d2;
  } __attribute__((packed));
  //perform invocation
  switch (type) {
   case MEMBER_FUNC: {
    const auto proxy = (R (*)(void *const, member_func_t, A))getFunctionProxy<A>();
    return proxy(
     CX::union_cast<JObject *>(clazzOrInst),
     CX::union_cast<member_func_t>(CX::member_ptr_align_t{fnPtr, adj}),
     args
    );
   }
   case STATIC_FUNC: {
    const auto proxy = (R (*)(static_func_t, A))getFunctionProxy<A>();
    return proxy(CX::union_cast<static_func_t>(fnPtr), args);
   }
   case REGISTER_NATIVES_FUNC: {
    const auto argc = descriptor->nargs - 2;
    void * values[descriptor->nargs];
    values[0] = new JNIEnv*;
    values[1] = new jobject*;
    *((JNIEnv **)values[0]) = &((const Jvm *)vm)->getJniEnv();
    *((jobject *)values[1]) = (jobject)clazzOrInst;
    //set up arguments
    const auto resolverOffset = CX::IsSame<arg_t, jvalue>::value ? argc : 0;
    for (unsigned int i = 0; i < argc; i++) {
     values[i + 2] = ((void * (*)(A))resolvers[i + resolverOffset])(args);
    }

    //Copy all primitives or object pointers into values (va_list only, jvalue* is done above)
    if constexpr (CX::IsSame<arg_t, CX::va_list_t>::value) {
     for (unsigned int i = 0; i < argc; i++) {
      const auto* ffi_arg_t = descriptor->arg_types[i + 2];
      _INTERNAL_INVOKE_VA_ARG(JByte, ffi_type_sint8)
      _INTERNAL_INVOKE_VA_ARG(JChar, ffi_type_uint16)
      _INTERNAL_INVOKE_VA_ARG(JShort, ffi_type_sint16)
      _INTERNAL_INVOKE_VA_ARG(JInt, ffi_type_sint32)
      _INTERNAL_INVOKE_VA_ARG(JFloat, ffi_type_float)
      _INTERNAL_INVOKE_VA_ARG(JLong, ffi_type_sint64)
      _INTERNAL_INVOKE_VA_ARG(JDouble, ffi_type_double)
//      _INTERNAL_INVOKE_VA_ARG(JObject *, ffi_type_pointer)
      if (ffi_arg_t == &ffi_type_pointer) {
       *((JObject **)values[i + 2]) = CX::safe_va_arg<JObject *>(args);
       continue;
      }
     }
     va_end(args);
    }
    if constexpr(CX::IsSame<R, void>::value) {
     ffi_call(descriptor, FFI_FN(fnPtr), nullptr, values);
     _INTERNAL_INVOKE_CLEANUP
     return;
    } else {
     using return_t = typename CX::select_if_true<sizeof(R) < sizeof(long), ffi_arg, R>::type;
     return_t result;
     ffi_call(descriptor, FFI_FN(fnPtr), &result, values);
     _INTERNAL_INVOKE_CLEANUP
     return (R)result;
    }
   }
   case STL_FUNC: {
    return ((R (*)(align_t, A))getFunctionProxy<A>())(CX::union_cast<align_t>(Data{fnPtr, stlFunc}), args);
   }
   case ARBITRARY_STL_FUNC: {
    auto vm_ref = const_cast<JavaVM *>(vm);
    JNIEnv * env;
    vm_ref->GetEnv((void **)&env, JNI_VERSION_1_8);
    return ((R (*)(align_t, const char *, JNIEnv *, void *, A))getFunctionProxy<A>())(
     CX::union_cast<align_t>(Data{fnPtr, stlFunc}),
     signature,
     env,
     clazzOrInst,
     args
    );
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

 template<typename T>
 void Jvm::setInvokeInterface() {
  static_assert(__is_base_of(InvokeInterface, T), "You must register a subtype of InvokeInterface!");
  setInvokeInterface(new T{*this});
 }

 template<typename T>
 void Jvm::setNativeInterface() {
  static_assert(__is_base_of(NativeInterface, T), "You must register a subtype of NativeInterface!");
  setNativeInterface(new T{*this});
 }

 template<typename T>
 void Jvm::setJvmtiInterface() {
  static_assert(__is_base_of(JvmtiInterface, T), "You must register a subtype of JvmtiInterface!");
  setJvmtiInterface(new T{*this});
 }

 template<typename T>
 void Jvm::setJniEnv() {
  static_assert(__is_base_of(JniEnv, T), "You must register a subtype of JniEnv!");
  setJniEnv(new T{*this});
 }

 template<typename T>
 void Jvm::setJvmtiEnv() {
  static_assert(__is_base_of(JvmtiEnv, T), "You must register a subtype of JvmtiEnv!");
  setJvmtiEnv(new T{*this});
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
  template<typename T>
  template<typename A>
  JObject * JClassBreeder<T, true>::constructorPredicate(const JavaVM * const vm, const char * const signature, A args) {
   JClass& descriptor = const_cast<JClass&>(T::descriptor);
   Jvm * const jvm = (Jvm *)const_cast<JavaVM *>(vm);
   for (auto& method : descriptor.functions) {
    if (strcmp(method->getSignature(), signature) == 0 && strcmp(method->getName(), "<init>") == 0) {
     const auto inst = method->nonVirtualInvoke<T *>(vm, &descriptor, &descriptor, args);
     JObject * baseInst;
     if constexpr(_CX::JniTypeBase<T>::hasComplexHierarchy) {
      baseInst = (JObject *)T::cast::cast(inst);
     } else {
      baseInst = (JObject *)inst;
     }
     (*jvm)[&descriptor].insert(baseInst);
     return baseInst;
    }
   }
   return nullptr;
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
  JObject * JClassBreeder<T, false>::constructorPredicate(const JavaVM * vm, const char * signature, A args) {
   throw std::runtime_error("FATAL: You cannot construct primitive types!");
   return nullptr;
  }

  template<typename T>
  constexpr void JClassBreeder<T, false>::assertCompliance() noexcept {
   //TODO ensure that the names dont contain [, L or ;
  }
 }

 //JValueUtil for JObject derived classes (see meta/method.h)
 namespace _CX {
  template<typename T>
  class JValueUtil<T, true> {

  public:
   inline static constexpr const bool isRegisteredResolver = true;
   using componentType = typename ComponentTypeResolver<T*>::type;

   static_assert(__is_base_of(JObject, componentType), "Illegal JNI function parameter type!");

   [[gnu::always_inline]]
   inline static componentType * getAArg(jvalue * values) {
    if constexpr(CastDefined<componentType>::value) {
     return componentType::cast::cast((JObject*)values->l);
    } else {
     return (componentType *)values->l;
    }
   }

   [[gnu::always_inline nodiscard]]
   inline static jvalue makeAArg(componentType * value) {
    jvalue jv;
    jv.l = value;
    return jv;
   }
  };
 }

 //JClass template members
 template<typename T>
 JClass::operator T() const {
  using component_t = typename CX::ComponentTypeResolver<T>::type;
  constexpr const auto
   downcast = __is_base_of(JClass, component_t),
   upcast = __is_base_of(component_t, JClass),
   jnicast = CX::MatchAny<component_t, _jobject, _jclass>::value;
  static_assert(
   downcast || upcast || jnicast,
   "JClass can only be upcast, downcast, or converted to _jobject or _jclass!"
  );
  auto ptr = const_cast<JClass *>(this);
  if constexpr(upcast || downcast) {
   return (T&)*ptr;
  } else if constexpr(jnicast) {
   return CX::union_cast<T>(this);
  }
 }

 template<typename T>
 JClass::JClass(uint32_t modifiers, _CX::JClassBreeder<T, true> breeder) noexcept :
  JObject(),
  constructV(&decltype(breeder)::template constructorPredicate<CX::va_list_t&>),
  constructA(&decltype(breeder)::template constructorPredicate<const jvalue *>),
  className(T::name),
  isArbitrary(false),
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
  constructV(&decltype(breeder)::template constructorPredicate<CX::va_list_t&>),
  constructA(&decltype(breeder)::template constructorPredicate<const jvalue *>),
  className(_CX::JniTypeBase<T>::signature),
  isArbitrary(false),
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

//_jobject template members
template<typename T>
_jobject::operator T() const {
 using component_t = typename CX::ComponentTypeResolver<T>::type;
 constexpr const auto downcast = __is_base_of(_jobject, component_t);
 static_assert(
  CX::MatchAny<component_t, FakeJni::JObject>::value || downcast,
  "jobject can only be converted to JObject and downcasted to derived JNI classes!"
 );
 return CX::union_cast<T>(const_cast<jobject>(this));
}

//_jclass template members
template<typename T>
_jclass::operator T() const {
 using component_t = typename CX::ComponentTypeResolver<T>::type;
 constexpr const auto upcast = __is_base_of(component_t, _jclass);
 static_assert(
  CX::MatchAny<component_t, FakeJni::JClass>::value || upcast,
  "jclass can only be converted to JClass upcast to jobject!"
 );
 return CX::union_cast<T>(const_cast<jclass>(this));
}

//Clean up internal macros
#undef _ASSERT_FIELD_JNI_COMPLIANCE
#undef _JFIELDID_GET
#undef _INTERNAL_INVOKE_VA_ARG
#undef _INTERNAL_INVOKE_CLEANUP
