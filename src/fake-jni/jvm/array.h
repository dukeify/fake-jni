#pragma once

//Users may use this macro to register the component type for their
//custom array definitions (see bottom of file)
#define REGISTER_ARRAY_TYPE(arrayType, componentType) \
namespace _CX {\
 template<>\
 class ArrayTypeBase<arrayType> {\
 public:\
  using type = componentType;\
 };\
 template<>\
 class JniTypeBase<arrayType> {\
 private:\
  inline static constexpr const char arrayPrefix[] = "[";\
 public:\
  inline static constexpr const bool isRegisteredType = true;\
  inline static constexpr const auto signature = CX::Concat<arrayPrefix, JniTypeBase<componentType>::signature>::result;\
 };\
}

//Internal macro
#define _DEFINE_ARRAY_TYPE(fake_type, jni_type)\
class fake_type: public JArray<jni_type> {\
public:\
 inline static constexpr const auto name = _CX::JniTypeBase<jni_type>::signature;\
 static const JClass descriptor;\
 explicit fake_type(const JInt size): JArray<jni_type>(size) {}\
 const JClass * getDescriptor() noexcept override {\
  return &descriptor;\
 }\
};\
const JClass fake_type::descriptor {\
 {&fake_type::length, "length", JFieldID::PUBLIC}\
};\
namespace _CX {\
 template<>\
 class ArrayTypeBase<fake_type> {\
 public:\
  using type = typename ArrayTypeBase<jni_type>::type;\
 };\
 template<>\
 class JniTypeBase<fake_type> {\
 public:\
  inline static constexpr const bool isRegisteredType = true;\
  inline static constexpr const auto signature = JniTypeBase<jni_type>::signature;\
 };\
}

namespace FakeJni {
 namespace _CX {
  template<typename>
  class ArrayTypeBase;
 }

 //TODO add fallback functionality for when non-trivial arrays are requested:
 // they should automatically default to JObjectArray
 template<typename T>
// class JArray: public T, public NativeObject<JArray<T>> {
 class JArray: public T, public NativeObject<JArray<T>> {
 private:
  using meta = _CX::ArrayTypeBase<T>;

 public:
  inline static constexpr const auto name = _CX::JniTypeBase<T>::signature;
  static const JClass descriptor;

  using componentType = typename meta::type;
  const JInt length;
  componentType * const array;

  explicit JArray(const JArray<T> &arr) : JArray(arr.length) {
   memcpy(array, arr.array, length);
  };

  explicit JArray(const JInt size):
   T(),
   NativeObject<JArray<T>>::NativeObject(),
   length(size),
   array(new componentType[size])
  {
   static_assert(std::is_base_of<_jarray, T>::value, "T must be derived from _jarray!");
  }

  virtual ~JArray() {
   delete[] array;
  }

  componentType& operator[](const JInt i) {
   if (i >= length) {
    throw std::out_of_range("Requested index out of range!");
   }
   return array[i];
  }

  virtual const JClass * getDescriptor() noexcept = 0;
 };

 //Register JNI array component type and signature associations
 REGISTER_ARRAY_TYPE(_jbooleanArray, JBoolean)
 REGISTER_ARRAY_TYPE(_jbyteArray, JByte)
 REGISTER_ARRAY_TYPE(_jcharArray, JChar)
 REGISTER_ARRAY_TYPE(_jshortArray, JShort)
 REGISTER_ARRAY_TYPE(_jintArray, JInt)
 REGISTER_ARRAY_TYPE(_jfloatArray, JFloat)
 REGISTER_ARRAY_TYPE(_jlongArray, JLong)
 REGISTER_ARRAY_TYPE(_jdoubleArray, JDouble)
 REGISTER_ARRAY_TYPE(_jobjectArray, JObject)

 //Define and register array types for all fake-jni implemented arrays
 //Uses the JNI array type and component type associations (see above)
 _DEFINE_ARRAY_TYPE(JBooleanArray, _jbooleanArray)
 _DEFINE_ARRAY_TYPE(JByteArray, _jbyteArray)
 _DEFINE_ARRAY_TYPE(JCharArray, _jcharArray)
 _DEFINE_ARRAY_TYPE(JShortArray, _jshortArray)
 _DEFINE_ARRAY_TYPE(JIntArray, _jintArray)
 _DEFINE_ARRAY_TYPE(JFloatArray, _jfloatArray)
 _DEFINE_ARRAY_TYPE(JLongArray, _jlongArray)
 _DEFINE_ARRAY_TYPE(JDoubleArray, _jdoubleArray)
 _DEFINE_ARRAY_TYPE(JObjectArray, _jobjectArray)

 //TODO user defined arrays
 //Users may use this macro to define their own array types to be used both
 //in fake-jni and through the JNI
 #define DEFINE_ARRAY_TYPE(type)
}