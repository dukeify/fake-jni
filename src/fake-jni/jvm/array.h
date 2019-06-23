#pragma once

//Internal macro
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
   inline static constexpr const char arrayPrefix[] = "[";\
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
}\
const JClass fake_object::descriptor {\
 {&fake_object::length, "length"}\
};

namespace FakeJni {
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
  JArray(TS... ts) : JArray(sizeof...(TS)) {
   static_assert((std::is_same<T, TS>::value && ...), "Arrays can only hold one type!");
   if constexpr(length > 0) {
    const T temp[] = {ts...};
    memcpy(array, temp, length);
   }
  }

  explicit JArray(const JArray<T> &array) : JArray(array.length) {
   memcpy(array, array.array, length);
  }

  explicit JArray(const JInt size) : length(size), array(new component[size]) {
   static_assert(std::is_base_of<_jarray, T>::value, "T must be derived from _jarray!");
  }

  virtual ~JArray() {
   delete[] array;
  }

  virtual JInt getLength() {
//   return (JInt)ceil(length / sizeof(T));
   return length;
  }

  component& operator[](const JInt i) {
   if (i > length) {
    throw std::out_of_range("Requested index out of range!");
   }
   return array[i];
  }
 };

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