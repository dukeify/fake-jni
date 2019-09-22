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
  private:\
   static constexpr const char arrayPrefix[] = "[";\
  public:\
   static constexpr const bool isRegisteredType = true;\
   static constexpr const bool isClass = __is_class(fake_object);\
   static constexpr const auto signature = CX::Concat<arrayPrefix, JniTypeBase<component_type>::signature>::result;\
   static constexpr const bool hasComplexHierarchy = CastDefined<fake_object>::value;\
  };\
  template<>\
  class JniTypeBase<JArray<fake_object>> {\
  public:\
   static constexpr const bool isRegisteredType = JniTypeBase<fake_object>::isRegisteredType;\
   static constexpr const bool isClass = JniTypeBase<fake_object>::isClass;\
   static constexpr const auto signature = JniTypeBase<fake_object>::signature;\
   static constexpr const bool hasComplexHierarchy = JniTypeBase<fake_object>::hasComplexHierarchy;\
  };\
  template<>\
  class JniTypeBase<jni_struct> {\
  public:\
   static constexpr const bool isRegisteredType = JniTypeBase<fake_object>::isRegisteredType;\
   static constexpr const bool isClass = JniTypeBase<fake_object>::isClass;\
   static constexpr const auto signature = JniTypeBase<fake_object>::signature;\
   static constexpr const bool hasComplexHierarchy = JniTypeBase<fake_object>::hasComplexHierarchy;\
  };\
 }\
 class fake_object : public JArray<fake_object> {\
 public:\
  template<typename T>\
  operator T() const;\
  using JArray<fake_object>::JArray;\
  inline static const JClass * getDescriptor() noexcept {\
   return JArray<fake_object>::getDescriptor();\
  }\
 };\
 template<typename T>\
 fake_object::operator T() const {\
  using component_t = typename CX::ComponentTypeResolver<T>::type;\
  constexpr const auto \
   downcast = __is_base_of(fake_object, component_t),\
   upcast = __is_base_of(component_t, fake_object),\
   jnicast = CX::MatchAny<component_t, _jobject, _jarray, jni_struct>::value;\
   static_assert(\
    downcast || upcast || jnicast,\
    #fake_object " can only be upcast, downcast, or converted to _jobject, _jarray or " #jni_struct\
   );\
   auto ptr = const_cast<fake_object *>(this);\
   if constexpr(downcast || upcast) {\
    return (T&)*ptr;\
   } else if constexpr(jnicast) {\
    return CX::union_cast<T>((JArrayBase *)this)();\
   }\
 }\
}\
template<>\
const FakeJni::JClass FakeJni::JArray<FakeJni::fake_object>::descriptor;

//User macros
#define DECLARE_NATIVE_ARRAY_DESCRIPTOR(fake_object) \
namespace FakeJni::_CX {\
 template<>\
 class JniArrayTypeBase<fake_object> {\
 public:\
  using component_t = fake_object;\
  static constexpr const auto isRegisteredType = true;\
 };\
 template<>\
 class JniTypeBase<JArray<fake_object>> {\
  static constexpr const char arrayPrefix[] = "[";\
 public:\
  static constexpr const bool isRegisteredType = true;\
  static constexpr const bool isClass = __is_class(JArray<fake_object>);\
  static constexpr const auto signature = CX::Concat<arrayPrefix, JniTypeBase<fake_object>::signature>::result;\
  static constexpr const bool hasComplexHierarchy = CastDefined<JArray<fake_object>>::value;\
 };\
}\
template<>\
const FakeJni::JClass FakeJni::JArray<fake_object>::descriptor;

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

 class JArrayBase : public JObject {
 public:
  template<typename T>
  operator T() const;

  virtual JInt getSize() const = 0;
 };

 //Immutable array base
 template<typename T>
 class JArray<const T> : public JArrayBase {
  friend T;
  friend JArray<T>;

 private:
  template<bool = _CX::JniArrayTypeBase<T>::isRegisteredType>
  struct ComponentTrait;

  template<>
  struct ComponentTrait<false> : CX::false_type {
  private:
   struct AssertionError {
    constexpr AssertionError() noexcept {
     //Will always fail
     static_assert(
      _CX::JniArrayTypeBase<T>::isRegisteredType,
      "You must declare the native array that you want to use in the global namespace using "
      "DECLARE_NATIVE_ARRAY_DESCRIPTOR(type), and then allocate it's descriptor somewhere in your program, using "
      "DEFINE_NATIVE_ARRAY_DESCRIPTOR(type), so that it may be registered on Jvm instances!"
     );
    }
   };

   static constexpr const AssertionError assert{};

  public:
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
  template<typename C>
  operator C() const;

  JArray(std::initializer_list<component>);
  explicit JArray(const JArray<T> & array);
  explicit JArray(JInt size);
  explicit JArray(component * arr, JInt length);
  JArray();
  virtual ~JArray();

  inline JInt getSize() const final {
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
 public:
  using JArray<const T>::JArray;
  using component = typename JArray<const T>::component;

  //fake-jni metadata
  static constexpr const auto name = _CX::JniTypeBase<JArray<T>>::signature;
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

 //ArrayBase template members
 template<typename T>
 JArrayBase::operator T() const {
  using component_t = typename CX::ComponentTypeResolver<T>::type;
  constexpr const auto
   downcast = __is_base_of(JArrayBase, component_t),
   upcast = __is_base_of(component_t, JArrayBase),
   jnicast = CX::MatchAny<component_t, _jobject, _jarray>::value;
  static_assert(
   downcast || upcast || jnicast,
   "JArray<T> can only be upcast, downcast, or converted to _jobject or _jarray!"
  );
  auto ptr = const_cast<JArrayBase *>(this);
  if constexpr(upcast || downcast) {
   return (T&)*ptr;
  } else if constexpr (jnicast) {
   return CX::union_cast<T>(this)();
  }
 }

 //Immutable JArray template members
 template<typename T>
 template<typename C>
 JArray<const T>::operator C() const {
  return JArrayBase::operator C();
 }

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
 {}

 template<typename T>
 JArray<const T>::JArray(component * const arr, const JInt length) : JArray(length) {
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