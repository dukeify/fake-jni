#pragma once

#include "jni.h"
#include "jvmti.h"

#include "fake-jni/util.h"

#include "cx/templates.h"
#include "cx/indirection.h"
#include "cx/classes.h"
#include "cx/idioms.h"
#include "cx/strings.h"

#include <dlfcn.h>
#include <initializer_list>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cstring>

//INTERNAL MACROS
#define _FETCH_JVM Jvm * const jvm = (Jvm *)vm;

#ifdef FAKE_JNI_DEBUG
#define _FETCH_JNI_ENV \
JniEnv *jenv = (JniEnv *)env;\
NativeInterface * const ni = (NativeInterface *)(env->functions);
#else
#define _FETCH_JNI_ENV \
NativeInterface * const ni = (NativeInterface *)(env->functions);
#endif

//TODO Clean up
#ifdef FAKE_JNI_DEBUG
#define _FETCH_JVMTI_ENV \
JvmtiEnv *jenv = (JvmtiEnv *)env;\
JvmtiInterface * const ji = (JvmtiInterface *)(env->functions);

#define _JVMTI_DEBUG(name) fprintf(jenv->getVM()->getLog(), "DEBUG: jvmtiInterface_1_::%s\n", #name);
#else
#define _FETCH_JVMTI_ENV \
JvmtiInterface * const ji = (JvmtiInterface *)(env->functions);

#define _JVMTI_DEBUG(name)
#endif

//No need to store an extra stack variable if debugging is disabled
#ifdef FAKE_JNI_DEBUG
#define _INVOKE_DEBUG(name) \
_FETCH_JVM \
fprintf(jvm->getLog(), "DEBUG: JNIInvokeInterface_::%s\n", #name);
#else
#define _INVOKE_DEBUG(name) _FETCH_JVM
#endif

//Same as _INVOKE_DEBUG
#ifdef FAKE_JNI_DEBUG
#define _NATIVE_DEBUG(name) fprintf(jenv->getVM()->getLog(), "DEBUG: JNINativeInterface_::%s\n", #name);
#else
#define _NATIVE_DEBUG(name)
#endif

//TODO remove these with the fix for #24
#ifdef _WIN32
#define STATIC_LIB_SUFFIX std::string(".lib")
 #define DYNAMIC_LIB_SUFFIX std::string(".dll")
#else
#define STATIC_LIB_SUFFIX std::string(".a")
#define DYNAMIC_LIB_SUFFIX std::string(".so")
#endif

//Internal JFieldID macros
#define _ASSERT_FIELD_JNI_COMPLIANCE \
static_assert(\
 std::is_base_of<JObject, T>::value || _CX::JniTypeBase<T>::isRegisteredType,\
 "Field type is not a valid JNI type!"\
);\
static_assert(\
 CX::ComponentTypeResolver<T>::indirectionCount == 0U,\
 "Registered field types cannot be pointers or references to a JNI type!"\
);

//Internal JMethodID macros
#define _ASSERT_JNI_FUNCTION_COMPLIANCE \
static_assert(\
 _CX::VerifyJniFunctionArguments<Args...>::verify(),\
 "Registered JNI functions may only accept JNI types and pointers to _jobject or derived classes!"\
);

#define _GET_AARG_MAP(t, member) \
template<>\
class JValueArgResolver<false, t> {\
public:\
 inline static constexpr const bool isRegisteredResolver = true;\
 [[gnu::always_inline]]\
 inline static t getAArg(jvalue *values) {\
  return (t)values->member;\
 }\
};

//Internal native array macros
//Used to define:
// 1. The JniTypeBase of the JNI array base
// 2. The JniTypeBase of the fake-jni array class
// 3. The fake-jni array class itself
#define _DEFINE_NATIVE_ARRAY(fake_object, jni_struct, component_type)\
class fake_object;\
namespace _CX {\
template<>\
 class JniArrayTypeBase<jni_struct> {\
 public:\
  using component_t = component_type;\
 };\
 template<>\
 class JniArrayTypeBase<fake_object> {\
 public:\
  using component_t = component_type;\
 };\
 template<>\
 class JniTypeBase<jni_struct> {\
 private:\
  static constexpr const char arrayPrefix[] = "[";\
 public:\
  static constexpr const bool isRegisteredType = true;\
  static constexpr const bool isClass = std::is_class<jni_struct>::value;\
  static constexpr const auto signature = CX::Concat<arrayPrefix, JniTypeBase<component_type>::signature>::result;\
  static constexpr const bool hasComplexHierarchy = false;\
 };\
}\
class fake_object: public NativeObject<fake_object>, public JArray<fake_object>, public jni_struct {\
public:\
 using cast = CX::ExplicitCastGenerator<fake_object, JClass, JObject>;\
 static constexpr const auto name = _CX::JniTypeBase<jni_struct>::signature;\
 static const JClass descriptor;\
 const JInt length = getLength();\
 using JArray<fake_object>::JArray;\
 inline static const JClass * getDescriptor() noexcept {\
  return &descriptor;\
 }\
};\
namespace _CX {\
 template<>\
 class JniTypeBase<fake_object> {\
 public:\
  static constexpr const bool isRegisteredType = true;\
  static constexpr const bool isClass = std::is_class<fake_object>::value;\
  static constexpr const auto signature = JniTypeBase<jni_struct>::signature;\
  static constexpr const bool hasComplexHierarchy = CastDefined<fake_object>::value;\
 };\
}

#define _DEFINE_NATIVE_ARRAY_DESCRIPTOR(fake_object) \
DEFINE_NATIVE_DESCRIPTOR(fake_object) {\
 {&fake_object::length, "length"}\
};

//FAKE-JNI API MACROS
#define DEFINE_JNI_TYPE(target, sig) \
namespace FakeJni::_CX {\
 template<>\
 class JniTypeBase<typename ComponentTypeResolver<target>::type> {\
 private:\
  using type = typename ComponentTypeResolver<target>::type;\
 public:\
  static constexpr const bool isRegisteredType = true;\
  static constexpr const bool isClass = std::is_class<type>::value;\
  static constexpr const char signature[] = sig;\
  static constexpr const bool hasComplexHierarchy = CastDefined<type>::value;\
 };\
}

#define DEFINE_CLASS_NAME(str) \
static constexpr const char name[] = str;\
static const JClass descriptor;\
inline static const JClass * getDescriptor() noexcept {\
 return &descriptor;\
}

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

#define DEFINE_NATIVE_DESCRIPTOR(clazz) \
inline const FakeJni::JClass clazz::descriptor

#define DEFINE_NATIVE_TYPE(clazz) \
DECLARE_NATIVE_TYPE(clazz)\
DEFINE_NATIVE_DESCRIPTOR(clazz)

//fake-jni type metadata templates
namespace FakeJni::_CX {
  using CX::ComponentTypeResolver;
  using CX::ExplicitCastGenerator;
  using CX::IsTemplateTemplate;
  using CX::TemplateIsSame;

  //Stub type for generating pointer-to-member types
  class AnyClass;

  //JNI Type metadata template
  template<typename>
  class JniTypeBase {
  public:
   static constexpr const bool isRegisteredType = false;
   static constexpr const bool isClass = false;
   static constexpr const char signature[] = "[INVALID_TYPE]";
   static constexpr const bool hasComplexHierarchy = false;
  };

  //Strip pointers off of JniTypeBase specializations and instantiations
  template<typename T>
  class JniTypeBase<T*> : public JniTypeBase<typename ComponentTypeResolver<T*>::type> {};

  //Strip references off of JniTypeBase specializations and instantiations
  template<typename T>
  class JniTypeBase<T&> : public JniTypeBase<typename ComponentTypeResolver<T&>::type> {};

  //Strip const qualifications off of JniTypeBase specializations and instantiations
  template<typename T>
  class JniTypeBase<const T> : public JniTypeBase<typename ComponentTypeResolver<const T>::type> {};

  //'cast' alias detection idiom
  //Negative case
  template<typename T, typename V = void>
  class CastDefined : public std::false_type {
  public:
   [[gnu::always_inline]]
   inline static void assertAliasCorrectness() {
    //Always false
    static_assert(
     std::is_same<V, void>::value,
     "Complex JNI types must define a 'cast' alias!"
    );
   }
  };

  //Positive case
  template<typename T>
  class CastDefined<T, std::void_t<typename T::cast>> : public std::true_type {
  private:
   template<typename>
   class TemplateTemplateDecomposer {};

   template<template<typename...> typename T1, typename... Args>
   class TemplateTemplateDecomposer<T1<Args...>> {
   public:
    [[gnu::always_inline]]
    inline static constexpr bool verifyParameters() {
     static_assert(
      TemplateIsSame<T1, ExplicitCastGenerator>::value,
      "Illegal type for 'cast' alias, should be 'ExplicitCastGenerator<...>'!"
     );
     static_assert(
      ((JniTypeBase<Args>::isRegisteredType && JniTypeBase<Args>::isClass) && ...),
      "You may only use registered JNI / FakeJni classes in an explicit casting route!"
     );
     return true;
    }
   };

  public:
   using cast = typename T::cast;

   [[gnu::always_inline]]
   inline static void assertAliasCorrectness() {
    static_assert(
     IsTemplateTemplate<cast>::value && TemplateTemplateDecomposer<cast>::verifyParameters(),
     "Illegal type for 'cast' alias, should be 'ExplicitCastGenerator<...>'!"
    );
   }
  };
}

//fake-jni integral types
namespace FakeJni {
 //Forward declare JObject to define metadata templates
 class JObject;

 using JVoid = void;
 using JBoolean = jboolean;
 using JByte = jbyte;
 using JChar = jchar;
 using JShort = jshort;
 using JInt = jint;
 using JFloat = jfloat;
 using JLong = jlong;
 using JDouble = jdouble;
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
DEFINE_JNI_TYPE(FakeJni::JObject, "Ljava/lang/Object;")

//fake-jni API declarations
namespace FakeJni {
 //JNI / JVMTI WRAPPER API
 class Jvm;

 class InvokeInterface : public JNIInvokeInterface_ {
 public:
  Jvm * const vm;

  explicit InvokeInterface(Jvm * vm);

  InvokeInterface(const InvokeInterface &) = delete;

  virtual ~InvokeInterface() = default;

  //misc.cpp
  virtual jint getEnv(Jvm *vm, void **penv, jint version) const;
  virtual jint destroyJavaVm(Jvm *vm) const;

  //thread.cpp
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
  Jvm * const vm;

  //native_constructor.h
  explicit NativeInterface(Jvm * vm);

  NativeInterface(const NativeInterface&) = delete;

  virtual ~NativeInterface() = default;

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
//  virtual jobject newObject(jclass, jmethodID, ...) const;
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

 class JvmtiInterface: public jvmtiInterface_1_ {
 private:
  //interface_inlined.cpp
  static jvmtiError _SetEventNotificationMode(jvmtiEnv* env, jvmtiEventMode mode, jvmtiEvent event_type, jthread event_thread, ...);

 public:
  Jvm * vm;

  //interface_constructor.cpp
  JvmtiInterface(Jvm * vm);

  JvmtiInterface(const JvmtiInterface&) = delete;

  virtual ~JvmtiInterface() = default;

  //thread.cpp
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

  //monitor.cpp
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

  //class.cpp
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

  //field.cpp
  virtual jvmtiError getFieldName(jvmtiEnv* env, jclass klass, jfieldID field, char** name_ptr, char** signature_ptr, char** generic_ptr) const;
  virtual jvmtiError getFieldDeclaringClass(jvmtiEnv* env, jclass klass, jfieldID field, jclass* declaring_class_ptr) const;
  virtual jvmtiError getFieldModifiers(jvmtiEnv* env, jclass klass, jfieldID field, jint* modifiers_ptr) const;
  virtual jvmtiError isFieldSynthetic(jvmtiEnv* env, jclass klass, jfieldID field, jboolean* is_synthetic_ptr) const;
  virtual jvmtiError setFieldAccessWatch(jvmtiEnv* env, jclass klass, jfieldID field) const;
  virtual jvmtiError clearFieldAccessWatch(jvmtiEnv* env, jclass klass, jfieldID field) const;
  virtual jvmtiError setFieldModificationWatch(jvmtiEnv* env, jclass klass, jfieldID field) const;
  virtual jvmtiError clearFieldModificationWatch(jvmtiEnv* env, jclass klass, jfieldID field) const;

  //method.cpp
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

  //misc.cpp
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

 class JniEnv: public JNIEnv {
 public:
  Jvm * const vm;
  NativeInterface * const ni;

 public:
  JniEnv(Jvm * vm, NativeInterface * ni) noexcept;
  virtual ~JniEnv() = default;
  virtual Jvm * getVM();
 };

 class JvmtiEnv: public jvmtiEnv {
 public:
  Jvm * const vm;
  JvmtiInterface * const ji;

  JvmtiEnv(Jvm * vm, JvmtiInterface * ji) noexcept;
  virtual ~JvmtiEnv() = default;
  virtual Jvm * getVM() final;
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
  void * handle;
  jint (* JNI_OnLoad_)(JavaVM * vm, void * reserved);
  void (* JNI_OnUnload_)(JavaVM * vm, void * reserved);
  jint (* Agent_OnAttach_)(JavaVM * vm, char * options, void * reserved);
  jint (* Agent_OnLoad_)(JavaVM * vm, char * options, void * reserved);
  void (* Agent_OnUnload_)(JavaVM * vm, char * options, void * reserved);

  bool determineIsStatic(const std::string &path);

 public:
  Jvm * const vm;
  const std::string path;
  const LibraryOptions options;
  const bool isStatic;

  Library(Jvm * vm, std::string path, LibraryOptions options);
  ~Library();

  void * lsym(const char * symbol);
  bool agentBound();
  bool jniBound();
  JInt agentAttach(char *options);
  JInt agentLoad(char *options);
  void agentUnload(char *options);
  jint jniLoad();
  void jniUnload();
 };

 class JClass;

 //JNI _jobject and java/lang/Object implementation
 class JObject: public _jobject {
 public:
  //Internal fake-jni native class metadata
  DEFINE_CLASS_NAME("java/lang/Object")

  explicit JObject(const JObject&) = delete;
  constexpr JObject() = default;
  virtual ~JObject() = default;
  virtual const JClass * getClass() noexcept;
 };

 //Wrapper template for constructor detection and registration
 template<typename T, typename... Args>
 class Constructor {
 public:
  Constructor() {
   //TODO
   static_assert(CX::HasConstructor<T, Args...>::value, "Tried to register non-existent constructor!");
  }

  [[gnu::always_inline]]
  inline static T * construct(Args... args) {
   return new T(args...);
  }
 };

 //Template glue code for field registration and access
 namespace _CX {
  //SFINAE templates to generate field accessors
  //Field accessor for mutable member fields
  template<typename...>
  class FieldAccessor;

  //Field accesor for mutable member fields
  template<typename T, typename F>
  class FieldAccessor<F (T::*)> {
  public:
   using type_t = F (T::* const);
   using erased_t = int (AnyClass::* const);

   [[gnu::always_inline]]
   inline static F get(void * inst, erased_t field) {
    return ((T * const)inst)->*((type_t)field);
   }

   [[gnu::always_inline]]
   inline static void set(void * const inst, erased_t field, void * const value) {
    ((T*)inst)->*((type_t)field) = *((F*)value);
   }
  };

  //Field accessor for immutable member fields
  template<typename T, typename F>
  class FieldAccessor<const F (T::*)> {
  public:
   using type_t = const F (T::* const);
   using erased_t = int (AnyClass::* const);

   [[gnu::always_inline]]
   inline static F get(void * inst, erased_t field) {
    return ((T*)inst)->*((type_t)field);
   }

   [[gnu::always_inline]]
   inline static void set(void * const inst, erased_t field, void * const value) {
    std::string error = "Attempted to write to immutable field: '";
    error += typeid(type_t).name();
    error += "'!";
    throw std::runtime_error(error.c_str());
   }
  };

  //Field accessor for mutable non-member fields
  template<typename F>
  class FieldAccessor<F*> {
  public:
   using type_t = F * const;
   using erased_t = void * const;

   [[gnu::always_inline]]
   inline static F get(erased_t field) {
    return *((type_t)field);
   }

   [[gnu::always_inline]]
   inline static void set(erased_t field, void * const value) {
    *((type_t)field) = *((type_t)value);
   }
  };

  //Field accessor for immutable non-member fields
  template<typename F>
  class FieldAccessor<const F*> {
  public:
   using type_t = const F * const;
   using erased_t = void * const;

   [[gnu::always_inline]]
   inline static F get(erased_t field) {
    return *((type_t)field);
   }

   [[gnu::always_inline]]
   inline static void set(erased_t field, void * value) {
    std::string error = "Attempted to write to immutable field: '";
    error +=  typeid(type_t).name();
    error += "'!";
    throw std::runtime_error(error.c_str());
   }
  };
 }

 //JNI base
 struct _jfieldID {};

 //fake-jni implementation
 class JFieldID: public _jfieldID {
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
  //TODO ensure that these are correct
  //FIELD_MODIFIERS = 223;
  enum Modifiers: uint32_t {
   PUBLIC = 1,
   PRIVATE = 2,
   PROTECTED = 4,
   STATIC = 8,
   FINAL = 16,
   VOLATILE = 64,
   TRANSIENT = 128
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

 //Template glue code for method registration and access
 namespace _CX {
  //Ensures that all type arguments are valid JNI parameters
  template<typename... Args>
  class VerifyJniFunctionArguments {
  public:
   [[gnu::always_inline]]
   inline static constexpr bool verify() {
    return  !((!VerifyJniFunctionArguments<Args>::verify()) || ...);
   }
  };

  template<typename T>
  class VerifyJniFunctionArguments<T> {
  public:
   [[gnu::always_inline]]
   inline static constexpr bool verify() {
    using resolver = ComponentTypeResolver<T>;
    if constexpr(std::is_class<typename resolver::type>::value) {
     return std::is_base_of<_jobject, typename resolver::type>::value && resolver::indirectionCount == 1U;
    } else {
     return JniTypeBase<T>::isRegisteredType && resolver::indirectionCount == 0U;
    }
   }
  };

  template<>
  class VerifyJniFunctionArguments<> {
  public:
   [[gnu::always_inline]]
   inline static constexpr bool verify() {
    return true;
   }
  };

  //C to C++ Vararg glue
  template<bool IsClass, typename T>
  class JValueArgResolver {
  public:
   inline static constexpr const bool isRegisteredResolver = false;
  };

  //JValueArgResolver for JObject derived classes
  template<typename T>
  class JValueArgResolver<true, T> {

  public:
   inline static constexpr const bool isRegisteredResolver = true;
   using componentType = typename ComponentTypeResolver<T*>::type;

   [[gnu::always_inline]]
   inline static componentType* getAArg(jvalue *values) {
    static_assert(std::is_base_of<_jobject, componentType>::value, "Illegal JNI function parameter type!");
    if constexpr(CastDefined<componentType>::value) {
     return componentType::cast::cast((JObject*)values->l);
    } else {
     return (componentType *)values->l;
    }
   }
  };

  //Function breeder for JValueArgResolver<typename>::getAArg(jvalue *)
  template<typename T>
  [[gnu::always_inline]]
  inline static T getAArg(jvalue *values) {
   using componentType = typename ComponentTypeResolver<T>::type;
   static_assert(
    JValueArgResolver<false, componentType>::isRegisteredResolver
    || JValueArgResolver<true, componentType>::isRegisteredResolver,
    "Illegal JNI function parameter type!"
   );
   return JValueArgResolver<std::is_class<componentType>::value, T>::getAArg(values);
  }

  template<
   typename T,
   bool IsPointer = std::is_pointer<T>::value,
   bool IsClass = std::is_class<T>::value,
   bool LargerThanInt = (sizeof(T) > sizeof(int))
  >
  class VArgResolver;

  //VArgResolver for pointer types
  template<typename T>
  class VArgResolver<T*, true, false, (sizeof(T*) > sizeof(int))> {
  public:
   [[gnu::always_inline]]
   inline static T* getVArg(va_list list) {
    return va_arg(list, T*);
   }
  };

  //VArgResolver for integral types smaller than int
  //Promotes to int and lossy-casts
  template<typename T>
  class VArgResolver<T, false, false, false> {
  public:
   [[gnu::always_inline]]
   inline static T getVArg(va_list list) {
    return (T)va_arg(list, int);
   }
  };

  //VArgResolver for integral types larger than int
  //Promotes to double and lossy-casts
  template<typename T>
  class VArgResolver<T, false, false, true> {
  public:
   [[gnu::always_inline]]
   inline static T getVArg(va_list list) {
    return (T)va_arg(list, double);
   }
  };

  //VArgResolver for object types
  //Consuming an object type off of a va_list is undefined behaviour
  template<typename T>
  class VArgResolver<T, false, true> {
  public:
   [[gnu::always_inline]]
   inline static T getVArg(va_list list) {
    //Static assertion will always fail in fault-conditions, uses sfinae to prevent
    //compiler errors in non-fault conditions, since static_assert(false, ...); will
    //always throw an error, even if the template is not instantiated
    static_assert(
     !std::is_class<T>::value,
     "Consuming an object type off of a va_list is undefined behaviour!\n"
     "Did you intend to consume a pointer-to-object type?"
    );
   }
  };

  template<typename T>
  [[gnu::always_inline]]
  inline static T getVArg(va_list args) {
   return VArgResolver<T>::getVArg(args);
  }

  //Type-to-member-name maps for the jvalue union
  _GET_AARG_MAP(JBoolean, z)
  _GET_AARG_MAP(JByte, b)
  _GET_AARG_MAP(JChar, c)
  _GET_AARG_MAP(JShort, s)
  _GET_AARG_MAP(JInt, i)
  _GET_AARG_MAP(JLong, j)
  _GET_AARG_MAP(JFloat, f)
  _GET_AARG_MAP(JDouble, d)

  template<auto, typename...>
  class FunctionAccessor {};

  //Entry invoker for member functions
  template<auto N, typename T, typename R, typename... Args>
  class FunctionAccessor<N, R (T::* const)(Args...)> {
  public:
   using argType = typename CX::TemplateTypeIterator<N - 1, Args...>::type;
   using functionType = R (T::* const)(Args...);
   using erasedType = void (AnyClass::* const)();

   template<typename... DecomposedVarargs>
   [[gnu::always_inline]]
   inline static R invokeV(void * const inst, erasedType func, va_list list, DecomposedVarargs... args) {
    return FunctionAccessor<N - 1, functionType>::template invokeV<argType, DecomposedVarargs...>(
     inst,
     func,
     list,
     getVArg<argType>(list),
     args...
    );
   }

   template<typename... DecomposedJValues>
   [[gnu::always_inline]]
   inline static R invokeA(void * const inst, erasedType func, jvalue *values, DecomposedJValues... args) {
    return FunctionAccessor<N - 1, functionType>::template invokeA<argType, DecomposedJValues...>(
     inst,
     func,
     values + 1,
     getAArg<argType>(values),
     args...
    );
   }
  };

  //Base invoker for member functions
  template<typename T, typename R, typename... Args>
  class FunctionAccessor<0, R (T::* const)(Args...)> {
  public:
   using erasedType = void (AnyClass::* const)();
   //Template pack should match Args
   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeV(void * const inst, erasedType func, va_list list, Args2... args) {
    va_end(list);
    return invokeA(inst, func, nullptr, args...);
   }

   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeA(void * const inst, erasedType func, jvalue *values, Args2... args) {
    static_assert(
     std::is_same<std::tuple<Args...>, std::tuple<Args2...>>::value,
     "Function argument list does not match base invoker arguments!"
    );
    return (((T*)inst)->*((R (T::*)(Args...))func))(args...);
   }
  };

  //Entry invoker for non-member functions
  template<auto N, typename R, typename... Args>
  class FunctionAccessor<N, R (* const)(Args...)> {
  public:
   using argType = typename CX::TemplateTypeIterator<N - 1, Args...>::type;
   using functionType = R (* const)(Args...);
   using erasedType = void (* const)();

   template<typename... DecomposedVarargs>
   [[gnu::always_inline]]
   inline static R invokeV(erasedType func, va_list list, DecomposedVarargs... args) {
    return FunctionAccessor<N - 1, functionType>::template invokeV<argType, DecomposedVarargs...>(
     func,
     list,
     getVArg<argType>(list),
     args...
    );
   }

   template<typename... DecomposedJValues>
   [[gnu::always_inline]]
   inline static R invokeA(erasedType func, jvalue * const values, DecomposedJValues... args) {
    return FunctionAccessor<N - 1, functionType>::template invokeA<argType, DecomposedJValues...>(
     func,
     values + 1,
     getAArg<argType>(values),
     args...
    );
   }
  };

  //Base invoker for non-member functions
  template<typename R, typename... Args>
  class FunctionAccessor<0, R (* const)(Args...)> {
  public:
   using erasedType = void (* const)();

   //Template pack should match Args
   template<typename... Args2>
   [[gnu::always_inline]]
   inline static R invokeV(erasedType func, va_list list, Args2... args) {
    va_end(list);
    return invokeA(func, nullptr, args...);
   }

   template<typename... Args2>
   inline static R invokeA(erasedType func, jvalue *values, Args2... args) {
    static_assert(
     std::is_same<std::tuple<Args...>, std::tuple<Args2...>>::value,
     "Function argument list does not match base invoker arguments!"
    );
    return ((R (*)(Args...))func)(args...);
   }
  };

  //SFINAE functions to generate JNI function signatures
  //Fold expression evaluators
  //Evaluator for non-empty fold
  template<typename... Args>
  class EvaluateFold {
  public:
   inline static constexpr const auto fold = CX::Concat<(JniTypeBase<Args>::signature, ...)>::result;
  };

  //Evaluator for empty folds
  template<>
  class EvaluateFold<> {
  public:
   inline static constexpr const char fold[] = "";
  };

  template<bool IsConstructor, typename R, typename... Args>
  class SignatureGenerator;

  //Generator for regular functions
  template<typename R, typename... Args>
  class SignatureGenerator<false, R, Args...> {
  public:
   inline static constexpr const char
    prefix[] = "(",
    suffix[] = ");";
   inline static constexpr const auto signature =
    CX::Concat<prefix, EvaluateFold<Args...>::fold, suffix, JniTypeBase<R>::signature>::result;
  };

  //Generator for constructors
  template<typename R, typename... Args>
  class SignatureGenerator<true, R, Args...> {
  public:
   inline static constexpr const char suffix[] = ");V";
   inline static constexpr const auto signature =
    CX::Concat<SignatureGenerator<false, R, Args...>::prefix, EvaluateFold<Args...>::fold, suffix>::result;
  };
 }

 //JNI base
 struct _jmethodID {};

 //fake-jni implementation
 class JMethodID: public _jmethodID, private JNINativeMethod {
 private:
  using staticFunc_t = void (* const)();
  using memberFunc_t = void (_CX::AnyClass::* const)();

  //Describes encapsulated function, not JVM modifier
  const bool isStatic;
  //Describes JVM modifier
  //A modifier of 0 indicates a native method registered through `registerNatives`
  const uint32_t modifiers;
  union {
   staticFunc_t staticFunc;
   memberFunc_t memberFunc;
  };
  void
  (* const proxyFuncV)(),
  (* const proxyFuncA)();

 public:
  //Currently JVM modifiers do nothing
  //METHOD_MODIFIERS = 3391;
  enum Modifiers: uint32_t {
   PUBLIC = 1,
   PRIVATE = 2,
   PROTECTED = 4,
   STATIC = 8,
   FINAL = 16,
   SYNCHRONIZED = 32,
   NATIVE = 256,
   ABSTRACT = 1024,
   STRICT = 2048
  };

  //Constructor for delegate constructors
  template<typename R, typename... Args>
  JMethodID(R (* const func)(Args...), const uint32_t modifiers) noexcept;
  //constructor for static functions
  template<typename R, typename... Args>
  JMethodID(R (* const func)(Args...), const char * const name, const uint32_t modifiers) noexcept;
  //Constructor for member methods
  template<typename T, typename R, typename... Args>
  JMethodID(R (T::* const func)(Args...), const char * const name, const uint32_t modifiers) noexcept;

  inline const char * getName() const noexcept {
   return name;
  }

  inline const char * getSignature() const noexcept {
   return signature;
  }

  inline uint32_t getModifiers() const noexcept {
   return modifiers;
  }

  bool operator ==(JMethodID &mid) const noexcept;
  template<typename R>
  [[gnu::always_inline]]
  R invoke(void * inst, va_list list) const;
  template<typename R>
  [[gnu::always_inline]]
  R invoke(void * inst, jvalue * values);
 };

 //Forward declare glue type for JClass construction
 namespace _CX {
  class ClassDescriptorElement;
 }

 //TODO register java.lang.Class functions for JClass if they are ever required
 class JClass : public JObject, public _jclass {
 private:
  const JClass * const copyConstructed;

  //Private constructor for JClass descriptor
  JClass(std::nullptr_t) noexcept;

 protected:
  //Property associations
  AllocStack<JMethodID *> * functions;
  AllocStack<JFieldID *> * fields;

  //TODO
  constexpr bool isNameCompliant(const char * name) const noexcept {
   return true;
  }

 public:
  //Internal fake-jni native class metadata
  DEFINE_CLASS_NAME("java/lang/Class")

  //TODO ensure that these are correct
  //CLASS_MODIFIERS = 3103;
  //INTERFACE_MODIFIERS = 3087;
  enum Modifiers : uint32_t {
   //TODO
  };

  explicit JClass(JClass &) = delete;
  JClass(std::initializer_list<_CX::ClassDescriptorElement> list) noexcept;
  JClass(const JClass * clazz) noexcept;
  virtual ~JClass();

  bool registerMethod(JMethodID * mid) noexcept;
  bool registerField(JFieldID * fid) noexcept;
  virtual const JClass * getClass() noexcept final;
  virtual const char * getName() noexcept;
  //Object construction for c-varargs
  virtual JObject * newInstance(JavaVM * vm, const char * signature, va_list list) const;
  //Object construction for jvalue arrays
  virtual JObject * newInstance(JavaVM * vm, const char * signature, jvalue * values) const;
 };

 //Template glue code for native class registration
 //TODO eliminate the RegistrationHookGenerator
 namespace _CX {
  template<typename>
  class RegistrationHookGenerator;

  class ClassDescriptorElement {
  public:
   enum DescriptorType {
    MEMBER_FUNCTION,
    NON_MEMBER_FUNCTION,
    MEMBER_FIELD,
    NON_MEMBER_FIELD
   };

   const DescriptorType type;

   union {
    void (AnyClass::* const memberFunc)();
    void (* const nonMemberFunc)();
    int AnyClass::* const memberField;
    void * const nonMemberField;
   };

   const char * const name;
   const uint32_t modifiers;
   //TODO use capturing lambdas in place of RegistrationHookGenerator after converting to a shared library
   bool (* const processHook)(JClass * const, ClassDescriptorElement * const);
//   std::function<bool (JClass * const)> processHook;

   //Constructor for member functions
   template<typename R, typename T, typename... Args>
   ClassDescriptorElement(R (T::* const func)(Args...), const char * const name, uint32_t modifiers = JMethodID::PUBLIC) noexcept :
    type(MEMBER_FUNCTION),
    memberFunc((decltype(memberFunc))func),
    name(name),
    modifiers(modifiers),
    processHook(&RegistrationHookGenerator<decltype(func)>::process)
   {}

   //Constructor for non-member functions
   template<typename R, typename... Args>
   ClassDescriptorElement(R (* const func)(Args...), const char * const name, uint32_t modifiers = JMethodID::PUBLIC) noexcept :
    type(NON_MEMBER_FUNCTION),
    nonMemberFunc((decltype(nonMemberFunc))func),
    name(name),
    modifiers(modifiers),
    processHook(&RegistrationHookGenerator<decltype(func)>::process)
   {}

   //Constructor for constructors
   template<typename T, typename... Args>
   ClassDescriptorElement(Constructor<T, Args...> constructor, uint32_t modifiers = JMethodID::PUBLIC) noexcept :
    type(NON_MEMBER_FUNCTION),
    nonMemberFunc((decltype(nonMemberFunc))Constructor<T, Args...>::construct),
    name("<init>"),
    modifiers(modifiers),
    processHook(&RegistrationHookGenerator<void (* const)(Args...)>::process)
   {}

   //Constructor for member fields
   template<typename F, typename T>
   ClassDescriptorElement(F (T::* const field), const char * const name, uint32_t modifiers = JFieldID::PUBLIC) noexcept :
    type(MEMBER_FIELD),
    memberField((decltype(memberField))field),
    name(name),
    modifiers(modifiers),
    processHook(&RegistrationHookGenerator<decltype(field)>::process)
   {}

   //Constructor for non-member fields
   template<typename F>
   ClassDescriptorElement(F * const field, const char * const name, uint32_t modifiers = JFieldID::PUBLIC) noexcept :
    type(NON_MEMBER_FIELD),
    nonMemberField((decltype(nonMemberField))field),
    name(name),
    modifiers(modifiers),
    processHook(&RegistrationHookGenerator<decltype(field)>::process) {}

   [[gnu::always_inline]]
   inline bool process(JClass *const clazz) const {
    return processHook(clazz, const_cast<ClassDescriptorElement *>(this));
   }
  };

  //Generator for member functions
  template<typename R, typename T, typename... Args>
  class RegistrationHookGenerator<R (T::* const)(Args...)> {
  public:
   using type = R (T::* const)(Args...);

   [[gnu::always_inline]]
   inline static bool process(JClass * const clazz, ClassDescriptorElement * const elem) {
    return clazz->registerMethod(new JMethodID((type)elem->memberFunc, elem->name, elem->modifiers));
   }
  };

  //Generator for non-member functions
  template<typename R, typename... Args>
  class RegistrationHookGenerator<R (*const)(Args...)> {
  public:
   using type = R (*const)(Args...);

   [[gnu::always_inline]]
   inline static bool process(JClass * const clazz, ClassDescriptorElement * const elem) {
    if (strcmp(elem->name, "<init>") == 0) {
     //Register constructor
     return clazz->registerMethod(new JMethodID((type)elem->nonMemberFunc, elem->modifiers));
    } else {
     return clazz->registerMethod(new JMethodID((type)elem->nonMemberFunc, elem->name, elem->modifiers));
    }
   }
  };

  //Generator for member fields
  template<typename F, typename T>
  class RegistrationHookGenerator<F (T::* const)> {
  public:
   using type = F (T::* const);

   [[gnu::always_inline]]
   inline static bool process(JClass * const clazz, ClassDescriptorElement * const elem) {
    return clazz->registerField(new JFieldID((type)elem->memberField, elem->name, elem->modifiers));
   }
  };

  //Generator for non-member fields
  template<typename F>
  class RegistrationHookGenerator<F * const> {
  public:
   using type = F * const;

   [[gnu::always_inline]]
   inline static bool process(JClass * const clazz, ClassDescriptorElement * const elem) {
    return clazz->registerField(new JFieldID((type)elem->nonMemberField, elem->name, elem->modifiers));
   }
  };
 }

 //FAKE-JNI USER API
 //TODO all JVM calls should be blocking until execution completes to prevent race conditions from emerging
 class Jvm: public JavaVM {
 private:
  using jnienv_t = JniEnv * const;
  using jvmtienv_t = JvmtiEnv * const;

  using iinvoke_t = InvokeInterface * const;
  using inative_t = NativeInterface * const;
  using ijvmti_t = JvmtiInterface * const;

  FILE * const log;
  iinvoke_t invoke;
  inative_t native;
  ijvmti_t jvmti;
  jnienv_t jniEnv;
  jvmtienv_t jvmtiEnv;

  //TODO on the first invocation of any JNI, JNIEnv or JVMTI functions, set this flag to true
  bool running;

  std::vector<Library *> libraries;
  std::vector<JClass *> classes;
  //TODO if classloaders are ever implemented, this property will be handled by the ClassLoader model
  AllocStack<JObject *> instances{true};

 protected:
  static AllocStack<Jvm *> vms;

//  static char * generateJvmUuid();
  static void generateJvmUuid(char * uuid);

 public:
//  const char * uuid;
  char * const uuid;

  template<typename INVOKE, typename NATIVE, typename JVMTI, typename JNI_ENV, typename JVMTI_ENV>
  explicit Jvm(
   FILE * log,
   INVOKE * const invoke,
   NATIVE * const native,
   JVMTI * const jvmti,
   JNI_ENV * const jniEnv,
   JVMTI_ENV * const jvmtiEnv
  );
  virtual ~Jvm();

  inline virtual FILE * getLog() {
   return log;
  }

  inline virtual InvokeInterface * getInvokeInterface() {
   return invoke;
  }

  inline virtual NativeInterface * getNativeInterface() {
   return native;
  }

  inline virtual JvmtiInterface * getJvmtiInterface() {
   return jvmti;
  }

  inline virtual JniEnv * getJniEnv() {
   return jniEnv;
  }

  inline virtual JvmtiEnv * getJvmtiEnv() {
   return jvmtiEnv;
  }

  inline virtual AllocStack<JObject *>* getInstances() {
   return &instances;
  }

  inline virtual bool isRunning() {
   return running;
  }

  template<typename T>
  bool registerClass();
  template<typename T>
  bool unregisterClass();
  virtual Library * getLibrary(const std::string &path);
  virtual Library * attachLibrary(
   const std::string &rpath,
   const std::string &options,
   LibraryOptions loptions
  );
  virtual bool removeLibrary(const std::string &path, const std::string &options);
  virtual void start();
  virtual void destroy();
 };

 //TODO detect if function is virtual: if any derived classes, of previously defined native classes, contain
 // a virtual override for the base, then register a new java function association for the derived class and
 // the respective function signature and classpath
 template<typename T>
 class NativeObject: public JClass {
 private:
  static constexpr const auto deallocator = &_CX::Deallocator<T>::deallocate;

  template<typename A>
  JObject * construct(JavaVM * const vm, const char * const signature, A args) const;

 public:
  explicit NativeObject(NativeObject &) = delete;
  NativeObject() noexcept;
  virtual ~NativeObject();

  inline const char * getName() noexcept final {
   return T::name;
  }

  [[gnu::always_inline]]
  inline JObject * newInstance(JavaVM * const vm, const char * const signature, va_list list) const final {
   return construct(vm, signature, list);
  }

  [[gnu::always_inline]]
  inline JObject * newInstance(JavaVM * const vm, const char * const signature, jvalue * const values) const final {
   return construct(vm, signature, values);
  }
 };

 //Native array implementation
 namespace _CX {
  template<typename T>
  class JniArrayTypeBase;
 }

 //Utility base, not a registered fake-jni type
 template<typename T>
 class JArray {
 private:
  using meta = typename _CX::JniArrayTypeBase<T>;

  const size_t length;

 public:
  using component = typename meta::component_t;
  component * const array;

  template<typename... TS>
  JArray(TS... ts);
  explicit JArray(const JArray<T> &array);
  explicit JArray(const JInt size);
  virtual ~JArray();

  inline virtual JInt getLength() {
   return length;
  }

  component& operator[](JInt i);
 };

 //Actual fake-jni array types
 //Provides array types for all default JNI defined arrays and their Java counterparts
 _DEFINE_NATIVE_ARRAY(JBooleanArray, _jbooleanArray, JBoolean)
 _DEFINE_NATIVE_ARRAY(JByteArray, _jbyteArray, JByte)
 _DEFINE_NATIVE_ARRAY(JCharArray, _jcharArray, JChar)
 _DEFINE_NATIVE_ARRAY(JShortArray, _jshortArray, JShort)
 _DEFINE_NATIVE_ARRAY(JIntArray, _jintArray, JInt)
 _DEFINE_NATIVE_ARRAY(JFloatArray, _jfloatArray, JFloat)
 _DEFINE_NATIVE_ARRAY(JLongArray, _jlongArray, JLong)
 _DEFINE_NATIVE_ARRAY(JDoubleArray, _jdoubleArray, JDouble)
 _DEFINE_NATIVE_ARRAY(JObjectArray, _jobjectArray, JObject)

 //JNI _jstring and java/lang/String implementation
 class JString: public _jstring, public JCharArray, public virtual NativeObject<JString> {
 private:
  static const JString EMPTY_STR;

 public:
  DEFINE_CLASS_NAME("java/lang/String")
  using cast = typename CX::ExplicitCastGenerator<JString, JCharArray, JClass, JObject>;

  static constexpr JString * const EMPTY = const_cast<JString *>(&EMPTY_STR);

  const size_t slen;

  JString(const JString &str);
  JString(const JInt size);
  JString(const char * str);

  inline JInt getLength() override {
   return (JInt)slen;
  }

  bool operator==(const JString& str);
 };

 //JNI _jthrowable and java/lang/Throwable implementation
 //TODO this is unfinished
 using JThrowable = _jthrowable;

 //JNI jweak and java/lang/ref/WeakReference<T> implementation
 //TODO this is unfinished
 using JWeak = JObject;

 //Factory function for creating Jvm instances
 template<
  typename INVOKE = InvokeInterface,
  typename NATIVE = NativeInterface,
  typename JVMTI = JvmtiInterface,
  typename JNI_ENV = JniEnv,
  typename JVMTI_ENV = JvmtiEnv
 >
 Jvm * createJvm(FILE * log = stdout) {
  //TODO all static assertions
  static_assert(
   std::is_base_of<InvokeInterface, INVOKE>::value
   && std::is_base_of<NativeInterface, NATIVE>::value
   && std::is_base_of<JvmtiInterface, JVMTI>::value
   && std::is_base_of<JniEnv, JNI_ENV>::value
   && std::is_base_of<JvmtiEnv, JVMTI_ENV>::value
  );
  return new Jvm(
   log,
   (typename CX::ComponentTypeResolver<INVOKE>::type *)nullptr,
   (typename CX::ComponentTypeResolver<NATIVE>::type *)nullptr,
   (typename CX::ComponentTypeResolver<JVMTI>::type *)nullptr,
   (typename CX::ComponentTypeResolver<JNI_ENV>::type *)nullptr,
   (typename CX::ComponentTypeResolver<JVMTI_ENV>::type *)nullptr
  );
 }
}

//Create template metadata for all native objects defined in fake-jni
//All JClass descriptors are allocated in the actual library implementation
//JObject is declared before everything else
//DECLARE_NATIVE_TYPE(FakeJni::JObject)
DECLARE_NATIVE_TYPE(FakeJni::JClass)
DECLARE_NATIVE_TYPE(FakeJni::JString)
//DECLARE_NATIVE_TYPE(FakeJni::JThrowable)
//DECLARE_NATIVE_TYPE(FakeJni::JWeak)

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
  //TODO should we lift this restriction?
  static_assert(std::is_base_of<JObject, M>::value, "Encapsulating class is not derived from JObject!");
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
   const_cast<char *>(_CX::SignatureGenerator<true, R, Args...>::signature),
   nullptr
  },
  isStatic(true),
  modifiers(modifiers),
  staticFunc((staticFunc_t)func),
  proxyFuncV((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeV<>),
  proxyFuncA((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeA<>)
 {
  _ASSERT_JNI_FUNCTION_COMPLIANCE
 }

 template<typename R, typename... Args>
 JMethodID::JMethodID(R (* const func)(Args...), const char * const name, const uint32_t modifiers) noexcept :
  _jmethodID(),
  JNINativeMethod {
   const_cast<char *>(name),
   const_cast<char *>(_CX::SignatureGenerator<false, R, Args...>::signature),
   nullptr
  },
  isStatic(true),
  modifiers(modifiers),
  staticFunc((staticFunc_t)func),
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
   const_cast<char *>(name),
   const_cast<char *>(_CX::SignatureGenerator<false, R, Args...>::signature),
   nullptr
  },
  isStatic(false),
  modifiers(modifiers),
  memberFunc((memberFunc_t)func),
  proxyFuncV((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeV<>),
  proxyFuncA((void (*)())&_CX::FunctionAccessor<sizeof...(Args), decltype(func)>::template invokeA<>)
 {
  _ASSERT_JNI_FUNCTION_COMPLIANCE
 }

 template<typename R>
 R JMethodID::invoke(void * const inst, va_list list) const {
  if (!modifiers) {
   throw std::runtime_error("Functions linked through `registerNatives` are not supported yet!");
  } else {
   if (isStatic) {
    return ((R (*)(staticFunc_t, va_list))proxyFuncV)(staticFunc, list);
   } else {
    return ((R (*)(void * const, memberFunc_t, va_list))proxyFuncV)(inst, memberFunc, list);
   }
  }
 }

 template<typename R>
 R JMethodID::invoke(void * const inst, jvalue * const values) {
  if (!modifiers) {
   throw std::runtime_error("Functions linked through `registerNatives` are not supported yet!");
  } else {
   if (isStatic) {
    return ((R (*)(staticFunc_t, jvalue *))proxyFuncA)(staticFunc, values);
   } else {
    return ((R (*)(void * const, memberFunc_t, jvalue *))proxyFuncA)(inst, memberFunc, values);
   }
  }
 }

 //Jvm template members
 template<typename INVOKE, typename NATIVE, typename JVMTI, typename JNI_ENV, typename JVMTI_ENV>
 Jvm::Jvm(
  FILE * log,
  INVOKE * const invoke,
  NATIVE * const native,
  JVMTI * const jvmti,
  JNI_ENV * const jniEnv,
  JVMTI_ENV * const jvmtiEnv
 ) :
  JavaVM(),
  log(log),
  invoke(new INVOKE(this)),
  native(new NATIVE(this)),
  jvmti(new JVMTI(this)),
  jniEnv(new JNI_ENV(this, this->native)),
  jvmtiEnv(new JVMTI_ENV(this, this->jvmti)),
  running(false),
  uuid(new char[33])
 {
  generateJvmUuid(uuid);
  functions = invoke;
  vms.pushAlloc(this);
  registerClass<JClass>();
 }

 template<typename T>
 bool Jvm::registerClass() {
  static_assert(
   std::is_base_of<JClass, T>::value,
   "Only native classes may be passed to registerClass!"
  );
  JClass *clazz = const_cast<JClass *>(&T::descriptor);
  if (std::find(classes.begin(), classes.end(), clazz) != classes.end()) {
#ifdef FAKE_JNI_DEBUG
   fprintf(
    log,
    "WARNING: Class '%s' is already registered on the JVM instance '%s'!\n",
    clazz->getName(),
    uuid
   );
#endif
   return false;
  } else {
   classes.push_back(clazz);
  }
  return true;
 }

 template<typename T>
 bool Jvm::unregisterClass() {
  static_assert(
   std::is_base_of<JClass, T>::value,
   "Only native classes may be passed to unregisterClass!"
  );
  auto clazz = const_cast<JClass *>(&T::descriptor);
  const auto end = classes.end();
  const auto ret = std::remove(classes.begin(), classes.end(), clazz);
#ifdef FAKE_JNI_DEBUG
  fprintf(
   log,
   "WARNING: Class '%s' is not registered on the JVM instance '%s'!\n",
   clazz->getName(),
   uuid
  );
#endif
  return end == ret;
 }

 //NativeObject template members
 template<typename T>
 template<typename A>
 JObject * NativeObject<T>::construct(JavaVM * const vm, const char * const signature, A args) const {
  Jvm * const jvm = (Jvm * const)vm;
  for (uint32_t i = 0; i < functions->getSize(); i++) {
   JMethodID * const method = (*functions)[i];
   if (strcmp(method->getSignature(), signature) == 0 && strcmp(method->getName(), "<init>") == 0) {
    const auto inst = method->invoke<T *>(nullptr, args);
    JObject * baseInst;
    if constexpr(_CX::JniTypeBase<T>::hasComplexHierarchy) {
     baseInst = T::cast::cast(inst);
    } else {
     baseInst = (JObject *)inst;
    }
    return jvm->getInstances()->pushAlloc(deallocator, baseInst);
   }
  }
  return nullptr;
 }

 template<typename T>
 NativeObject<T>::NativeObject() noexcept : JClass(T::getDescriptor()) {
//   static_assert(std::is_base_of<NativeObject, T>::value, "T must be derived from NativeObject<T>!");
  //Perform checks for complex hierarchy 'cast' alias member
  _CX::CastDefined<T>::assertAliasCorrectness();
 }

 template<typename T>
 NativeObject<T>::~NativeObject() = default;

 //JArray template members
 template<typename T>
 template<typename... TS>
 JArray<T>::JArray(TS... ts) : JArray(sizeof...(TS)) {
  static_assert((std::is_same<T, TS>::value && ...), "Arrays can only hold one type!");
  if constexpr(length > 0) {
   const T temp[] = {ts...};
   memcpy(array, temp, length);
  }
 }

 template<typename T>
 JArray<T>::JArray(const JArray<T> &array) : JArray(array.length) {
  memcpy(array, array.array, length);
 }

 template<typename T>
 JArray<T>::JArray(const JInt size) :
  length(size),
  array(new component[size])
 {
  static_assert(std::is_base_of<_jarray, T>::value, "T must be derived from _jarray!");
 }

 template<typename T>
 JArray<T>::~JArray() {
  delete[] array;
 }

 template<typename T>
 typename JArray<T>::component& JArray<T>::operator[](const JInt i) {
  if (i > length) {
   throw std::out_of_range("Requested index out of range!");
  }
  return array[i];
 }
}