#pragma once

#include "fake-jni/jvm.h"

//Internal native array macros
#define _DEFINE_NATIVE_ARRAY(fake_object, jni_struct, component_type)\
namespace FakeJni {\
 class fake_object;\
 namespace _CX {\
  template<>\
  class JniArrayTypeBase<fake_object> {\
  public:\
   using component_t = component_type;\
   static constexpr const auto isRegisteredType = true;\
  };\
 }\
 namespace _CX {\
  template<>\
  class JniTypeBase<fake_object> {\
  public:\
   static constexpr const bool isRegisteredType = true;\
   static constexpr const bool isClass = __is_class(fake_object);\
   static constexpr const auto signature = JniTypeBase<component_type>::signature;\
   static constexpr const bool hasComplexHierarchy = CastDefined<fake_object>::value;\
  };\
 }\
 class fake_object : public JArray<fake_object>, public jni_struct {\
 public:\
  using JArray<fake_object>::JArray;\
  inline static const JClass * getDescriptor() noexcept {\
   return JArray<fake_object>::getDescriptor();\
  }\
 };\
}\
template<>\
const FakeJni::JClass FakeJni::JArray<FakeJni::fake_object>::descriptor;

//User macros
#define DEFINE_NATIVE_ARRAY_DESCRIPTOR(fake_object) \
template<>\
const FakeJni::JClass FakeJni::JArray<fake_object>::descriptor {\
 FakeJni::JClass::PUBLIC,\
 FakeJni::_CX::JClassBreeder<fake_object> {\
  {FakeJni::Field<&JArray<fake_object>::length> {}, "length"}\
 }\
};

namespace FakeJni {
 //Utility base, not a registered fake-jni type
 template<typename T>
 class JArray;

 //Immutable array base
 template<typename T>
 class JArray<const T> : public JObject {
  friend T;
  friend JArray<T>;

 private:
  template<bool = _CX::JniArrayTypeBase<T>::isRegisteredType>
  struct ComponentTrait;

  template<>
  struct ComponentTrait<false> : CX::false_type {
   using component_t = T;
  };

  template<>
  struct ComponentTrait<true> : CX::true_type {
   using component_t = typename _CX::JniArrayTypeBase<T>::component_t;
  };

 public:
  using component = typename ComponentTrait<>::component_t;

 private:
  JInt length;
  component * array;

  static JInt boundsCheck(JInt len);

 public:
  JArray(std::initializer_list<component>);
  explicit JArray(const JArray<T> & array);
  explicit JArray(JInt size);
  explicit JArray(T * arr, JInt length);
  JArray();
  virtual ~JArray();

  inline JInt getLength() const {
   return length;
  }

  inline component* getArray() const {
   return const_cast<JArray<const T>&>(*this).array;
  }

  const component& operator[](JInt i) const;
 };

 //Mutable array implementation
 template<typename T>
 class JArray : public JArray<const T> {
 private:
  static constexpr const char arrayPrefix[] = "[";

 public:
  using JArray<const T>::JArray;
  using component = typename JArray<const T>::component;

  //fake-jni metadata
  static constexpr const auto name = CX::Concat<arrayPrefix, _CX::JniTypeBase<T>::signature>::result;
  static const JClass descriptor;
  inline static const JClass * getDescriptor() noexcept {
   return &descriptor;
  }
  virtual const JClass & getClass() const noexcept override {
   return descriptor;
  }

  JArray<T>& operator=(const JArray<T> & arr) const;
  const component& operator[](JInt i) const;
  component& operator[](JInt i);
 };

 //Immutable JArray template members
 template<typename T>
 inline JInt JArray<const T>::boundsCheck(JInt len) {
  if (len < 0) {
   throw std::out_of_range("FATAL: Arrays cannot have a negative size!");
  }
  return len;
 }

 template<typename T>
 JArray<const T>::JArray(std::initializer_list<component> list) : JArray((JInt)list.size()) {
  size_t i = 0;
  for (auto element : list) {
   array[i++] = element;
  }
 }

 template<typename T>
 JArray<const T>::JArray(const JArray<T> & array) : JArray(array.length) {
  memcpy(array, array.array, length);
 }

 template<typename T>
 JArray<const T>::JArray(const JInt size) :
  length(boundsCheck(size)),
  array(new component[length])
 {
  static_assert(__is_base_of(_jarray, T), "T must be derived from _jarray!");
 }

 template<typename T>
 JArray<const T>::JArray(T * const arr, const JInt length) : JArray(length) {
  for (JInt i = 0; i < length; i++) {
   array[i] = arr[i];
  }
 }

 template<typename T>
 JArray<const T>::JArray() : JArray(0)
 {}

 template<typename T>
 JArray<const T>::~JArray() {
  delete[] array;
 }

 template<typename T>
 const typename JArray<const T>::component& JArray<const T>::operator[](const JInt i) const {
  return array[i];
 }

 //Mutable JArray template members
 template<typename T>
 const typename JArray<T>::component& JArray<T>::operator[](JInt i) const {
  return JArray<const T>::operator[](i);
 }

 template<typename T>
 typename JArray<T>::component& JArray<T>::operator[](const JInt i) {
  if (i > JArray<const T>::length) {
   throw std::out_of_range("Requested index out of range!");
  }
  return JArray<const T>::array[i];
 }

 template<typename T>
 JArray<T>& JArray<T>::operator=(const JArray<T> &arr) const {
  auto& ref = const_cast<JArray<T>&>(*this);
  if (ref.length != arr.length) {
   ref.length = arr.length;
   delete[] ref.array;
   ref.array = new component[ref.length];
  }
  for (JInt i = 0; i < ref.length; i++) {
   ref.array[i] = arr.array[i];
  }
  return ref;
 }
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