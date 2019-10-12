#pragma once

#include "fake-jni/internal/meta/meta.h"

#define _DEFINE_JNI_CLASS(name, base) \
class name : public base {\
public:\
 constexpr name(const name&) = delete;\
 constexpr name(const name&&) = delete;\
 constexpr name() = delete;\
};

//Inclusion ordering guard
namespace FakeJni::_CX {
 struct _InclusionOrderAssertion_ {
  template<bool Fail = false, typename... Args>
  constexpr _InclusionOrderAssertion_(Args...) noexcept {
   static_assert(Fail);
  }
 };
};

#if defined(_JAVASOFT_JNI_MD_H_) || defined(_JAVASOFT_JNI_H_) || defined(_JAVA_JVMTI_H_)
namespace FakeJni::_CX {
 constexpr const _InclusionOrderAssertion_ _{"Do not include JNI headers if you are using fake-jni!"};
};
#endif

//Internal macros
#define _DECLARE_JNI_CONVERSION_OPERATOR(jni_ptr_type)\
class _##jni_ptr_type : public _jobject {\
public:\
 constexpr _##jni_ptr_type(const _##jni_ptr_type&) = delete;\
 constexpr _##jni_ptr_type(const _##jni_ptr_type&&) = delete;\
 constexpr _##jni_ptr_type() = delete;\
 \
 template<typename T>\
 operator T() const;\
};

#define _DEFINE_JNI_CONVERSION_OPERATOR(fake_type, jni_ptr_type)\
template<typename T>\
_##jni_ptr_type::operator T() const {\
 using namespace FakeJni;\
 using component_t = typename CX::ComponentTypeResolver<T>::type;\
 constexpr const auto upcast = __is_base_of(component_t, _##jni_ptr_type);\
 static_assert(\
  CX::MatchAny<component_t, fake_type>::value || upcast,\
  #jni_ptr_type " can only be converted to " #fake_type "!"\
 );\
 return CX::union_cast<T>(const_cast<jni_ptr_type>(this));\
 \
}

//fake-jni API macros
#define DEFINE_JNI_TYPE(target, sig)\
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

//Required for jni.h
using jboolean = unsigned char;
using jchar = unsigned short;
using jshort = short;
using jfloat = float;
using jdouble = double;
using jint = int;
using jsize = jint;
using jbyte = signed char;

#ifdef _LP64
using jlong = long;
#else
using jlong = long long;
#endif

namespace FakeJni {
 //fake-jni integral types
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

class _jobject {
public:
 constexpr _jobject() = delete;
 constexpr _jobject(const _jobject&) = delete;
 constexpr _jobject(const _jobject&&) = delete;

 template<typename T>
 operator T() const;
};

_DECLARE_JNI_CONVERSION_OPERATOR(jclass)
_DECLARE_JNI_CONVERSION_OPERATOR(jthrowable)
_DECLARE_JNI_CONVERSION_OPERATOR(jstring)
_DECLARE_JNI_CONVERSION_OPERATOR(jweak)
_DECLARE_JNI_CONVERSION_OPERATOR(jarray)

_DEFINE_JNI_CLASS(_jbooleanArray, _jarray)
_DEFINE_JNI_CLASS(_jbyteArray, _jarray)
_DEFINE_JNI_CLASS(_jcharArray, _jarray)
_DEFINE_JNI_CLASS(_jshortArray, _jarray)
_DEFINE_JNI_CLASS(_jintArray, _jarray)
_DEFINE_JNI_CLASS(_jlongArray, _jarray)
_DEFINE_JNI_CLASS(_jfloatArray, _jarray)
_DEFINE_JNI_CLASS(_jdoubleArray, _jarray)
_DEFINE_JNI_CLASS(_jobjectArray, _jarray)

using jobject = _jobject *;
using jclass = _jclass *;

using jthrowable = _jthrowable *;
using jstring = _jstring *;
using jweak = _jweak *;

using jarray = _jarray *;
using jbooleanArray = _jbooleanArray *;
using jbyteArray = _jbyteArray *;
using jcharArray = _jcharArray *;
using jshortArray = _jshortArray *;
using jintArray = _jintArray *;
using jlongArray = _jlongArray *;
using jfloatArray = _jfloatArray *;
using jdoubleArray = _jdoubleArray *;
using jobjectArray = _jobjectArray *;

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

union jvalue {
 jboolean z;
 jbyte b;
 jchar c;
 jshort s;
 jint i;
 jlong j;
 jfloat f;
 jdouble d;
 jobject l;
};

enum jobjectRefType {
 JNIInvalidRefType = 0,
 JNILocalRefType = 1,
 JNIGlobalRefType = 2,
 JNIWeakGlobalRefType = 3
};

struct _jfieldID {};
using jfieldID = _jfieldID *;

struct _jmethodID {};
using jmethodID = _jmethodID *;

namespace FakeJni::_CX {
 template<> struct IsJniClass<_jobject> : CX::true_type {};
 template<> struct IsJniClass<_jclass> : CX::true_type {};
 template<> struct IsJniClass<_jthrowable> : CX::true_type {};
 template<> struct IsJniClass<_jstring> : CX::true_type {};
 template<> struct IsJniClass<_jarray> : CX::true_type {};
 template<> struct IsJniClass<_jbooleanArray> : CX::true_type {};
 template<> struct IsJniClass<_jbyteArray> : CX::true_type {};
 template<> struct IsJniClass<_jcharArray> : CX::true_type {};
 template<> struct IsJniClass<_jshortArray> : CX::true_type {};
 template<> struct IsJniClass<_jintArray> : CX::true_type {};
 template<> struct IsJniClass<_jlongArray> : CX::true_type {};
 template<> struct IsJniClass<_jfloatArray> : CX::true_type {};
 template<> struct IsJniClass<_jdoubleArray> : CX::true_type {};
 template<> struct IsJniClass<_jobjectArray> : CX::true_type {};
}

#define JNI_TYPES_ALREADY_DEFINED_IN_JNI_MD_H

#define JNIEXPORT __attribute__((visibility("default")))
#define JNIIMPORT JNIEXPORT
#define JNICALL

#define _JAVASOFT_JNI_MD_H_

#include "jni.h"
#include "jvmti.h"

