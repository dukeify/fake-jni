#pragma once

#include "fake-jni/jvm.h"

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
  static constexpr const bool isClass = __is_class(jni_struct);\
  static constexpr const auto signature = CX::Concat<arrayPrefix, JniTypeBase<component_type>::signature>::result;\
  static constexpr const bool hasComplexHierarchy = false;\
 };\
}\
class fake_object : public JArray<fake_object>, public jni_struct {\
public:\
 using cast = CX::ExplicitCastGenerator<fake_object, JClass, JObject>;\
 static constexpr const auto name = _CX::JniTypeBase<jni_struct>::signature;\
 static const JClass descriptor;\
 using JArray<fake_object>::JArray;\
 inline static const JClass * getDescriptor() noexcept {\
  return &descriptor;\
 }\
 virtual const JClass & getClass() const noexcept override {\
  return descriptor;\
 }\
};\
namespace _CX {\
 template<>\
 class JniTypeBase<fake_object> {\
 public:\
  static constexpr const bool isRegisteredType = true;\
  static constexpr const bool isClass = __is_class(fake_object);\
  static constexpr const auto signature = JniTypeBase<jni_struct>::signature;\
  static constexpr const bool hasComplexHierarchy = CastDefined<fake_object>::value;\
 };\
}

#define _DEFINE_NATIVE_ARRAY_DESCRIPTOR(fake_object) \
DEFINE_NATIVE_DESCRIPTOR(fake_object) \
 {Field<&fake_object::length> {}, "length"}\
END_NATIVE_DESCRIPTOR

namespace FakeJni {
 //Utility base, not a registered fake-jni type
 template<typename T>
 class JArray : public JObject {
  friend T;

 private:
  using meta = typename _CX::JniArrayTypeBase<T>;

  const JInt length;

  static JInt boundsCheck(JInt len);

 public:
  using component = typename meta::component_t;
  component * const array;

  template<typename E>
  JArray(std::initializer_list<E>);
  explicit JArray(const JArray<T> &array);
  explicit JArray(JInt size);
  explicit JArray();
  virtual ~JArray();

  inline virtual JInt getLength() const {
   return length;
  }

  component& operator[](JInt i);
  const component& operator[](JInt i) const;
 };

 //JArray template members
 template<typename T>
 inline JInt JArray<T>::boundsCheck(JInt len) {
  if (len < 0) {
   throw std::out_of_range("FATAL: Arrays cannot have a negative size!");
  }
  return len;
 }

 template<typename T>
 template<typename E>
 JArray<T>::JArray(std::initializer_list<E> list) : JArray((JInt)list.size()) {
//  static_assert((CX::IsSame<T, TS>::value && ...), "Arrays can only hold one type!");
//  if constexpr(length > 0) {
//   const T temp[] = {ts...};
//   memcpy(array, temp, length);
//  }
  for (unsigned long i = 0; i < list.size(); i++) {
   array[i] = list[i];
  }
 }

 template<typename T>
 JArray<T>::JArray(const JArray<T> &array) : JArray(array.length) {
  memcpy(array, array.array, length);
 }

 template<typename T>
 JArray<T>::JArray(const JInt size) :
  length(boundsCheck(size)),
  array(new component[length])
 {
  static_assert(__is_base_of(_jarray, T), "T must be derived from _jarray!");
 }

 template<typename T>
 JArray<T>::JArray() : JArray(0)
 {}

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

 template<typename T>
 const typename JArray<T>::component& JArray<T>::operator[](const JInt i) const {
  return (*const_cast<JArray<T> *>(this))[i];
 }

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
}