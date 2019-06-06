#pragma once

#include <cstring>
#include "cx/strings.h"

//TODO JClass definitions
#define DEFINE_ARRAY_TYPE(arrayType, componentType) \
template<>\
class ArrayTypeBase<arrayType> {\
public:\
 using type = componentType;\
 inline static constexpr const char * const name = #arrayType;\
};
//TODO what is wrong with CX::concat
//};\
//template<>\
//class JniTypeBase<arrayType> {\
//public:\
// inline static constexpr const bool isRegisteredType = true;\
// inline static constexpr const char * signature = CX::concat<"[", JniTypeBase<componentType>::signature>();\
//};

#define DEFINE_JNI_TYPE(type, sig) \
template<>\
class JniTypeBase<type> {\
public:\
 inline static constexpr const bool isRegisteredType = true;\
 inline static constexpr const char * signature = #sig;\
};

//TODO what is wrong with CX::concat
#define DEFINE_NATIVE_TYPE(clazz) \
namespace FakeJni::_CX {\
 template<>\
 class JniTypeBase<clazz> {\
 public:\
  inline static constexpr const bool isRegisteredType = true;\
  inline static constexpr const char * const signature = CX::concat<"L", clazz::name, ";">();\
 };\
};

namespace FakeJni {
 //Stowaway namespace for template junk
 namespace _CX {
  template<typename T>
  class JniTypeBase {
  public:
   const bool isRegisteredType = false;
  };

  //All primitives here
  DEFINE_JNI_TYPE(JVoid, V)
  DEFINE_JNI_TYPE(JBoolean, Z)
  DEFINE_JNI_TYPE(JByte, B)
  DEFINE_JNI_TYPE(JChar, C)
  DEFINE_JNI_TYPE(JShort, S)
  DEFINE_JNI_TYPE(JInt, I)
  DEFINE_JNI_TYPE(JFloat, F)
  DEFINE_JNI_TYPE(JLong, J)
  DEFINE_JNI_TYPE(JDouble, D)

  //TODO all other classes will have to be initialized in the _CX namespace block below JClass
  DEFINE_JNI_TYPE(JClass, L)

  //SFINAE functions to generate JNI function signatures
  template<typename R, typename M, typename... Args>
  constexpr const char * generateSignature(R (M::* const func)(Args...)) noexcept {
   using namespace CX;
   return concat<"(", (JniTypeBase<Args>::signature, ...), ");", JniTypeBase<R>::signature>();
  }

  template<typename R, typename... Args>
  constexpr const char * generateSignature(R (* const func)(Args...)) noexcept {
   using namespace CX;
   return concat<"(", (JniTypeBase<Args>::signature, ...), ");", JniTypeBase<R>::signature>();
  }

  //SFINAE functions to generate JNI field signatures
  template<typename T, typename M>
  constexpr const char * generateSignature(T (M::* const field)()) noexcept {
   return JniTypeBase<T>::signature;
  }

  template<typename T>
  constexpr const char * generateSignature(T field) noexcept {
   return JniTypeBase<T>::signature;
  }
 }

 struct _jmethodID {};
 struct _jfieldID {};

 class JMethodID: public _jmethodID, private JNINativeMethod {
 private:
  union {
   void (* const staticFunc)();
   void (JObject::* const memberFunc)();
  };

  const bool isStatic;

 public:
  const bool isJvmStatic;

  template<typename T>
  constexpr JMethodID(const char *name, const bool isJvmStatic, void (T::* const func)()):
   _jmethodID(),
   JNINativeMethod { name, _CX::generateSignature(func), nullptr },
   memberFunc(func),
   isStatic(false),
   isJvmStatic(isJvmStatic)
  {
   static_assert(std::is_base_of<JObject, T>::value, "Function does not originate from a JObject derived class!");
  }

  template<typename F>
  constexpr JMethodID(const char *name, const bool isJvmStatic, const F func):
   _jmethodID(),
   JNINativeMethod { name, _CX::generateSignature(func), nullptr },
   staticFunc(func),
   isStatic(true),
   isJvmStatic(isJvmStatic)
  {
   static_assert(std::is_function<F>::value, "Argument is not a function!");
  }

  const char * getName() {
   return name;
  }

  const char * getSignature() {
   return signature;
  }

  //TODO
  template<typename R>
  R invoke(JObject *obj, va_list args) {
   if (isStatic) {
//    return staticFunc();
   } else {
//    return obj->*memberFunc(...);
   }
   throw std::runtime_error("unimplemented");
  }
 };

#define ASSERT_FIELD_JNI_COMPLIANCE static_assert(\
 std::is_base_of<JObject, T>::value || _CX::JniTypeBase<T>::isRegisteredType,\
 "Field type is not a valid JNI type!"\
 );

//TODO
 class JObject: public _jobject {
 protected:
  explicit constexpr JObject(JClass * const clazz): clazz(clazz)
  {}

 public:
  const JClass * const clazz;
 };

 class JFieldID: public _jfieldID {
 private:
  union {
   void * const staticProp;
   int (JObject::* const memberProp)();
  };

  const bool isStatic;

 public:
  const char * const name;
  const char * const signature;
  const bool isJvmStatic;

  template<typename T, typename M>
  constexpr JFieldID(const char * const name, const bool isJvmStatic, T M::* const member):
   _jfieldID(),
   isStatic(false),
   name(name),
   signature(_CX::generateSignature(member)),
   isJvmStatic(isJvmStatic)
  {
   ASSERT_FIELD_JNI_COMPLIANCE
   static_assert(std::is_base_of<JObject, M>::value, "Encapsulating class is not derived from JObject!");
  }

  template<typename T>
  constexpr JFieldID(const char * const name, const bool isJvmStatic, T * const staticMember):
   _jfieldID(),
   isStatic(true),
   name(name),
   signature(_CX::generateSignature(staticMember)),
   isJvmStatic(isJvmStatic)
  {
   ASSERT_FIELD_JNI_COMPLIANCE
  }

  //TODO
  template<typename T>
  T get(JObject *obj) {
   if (isStatic) {
//    return *staticProp;
   } else {
//    return obj->*memberProp;
   }
   throw std::runtime_error("unimplemented");
  }

  //TODO
  template<typename T>
  void set(JObject *obj, T value) {
   if (isStatic) {
//    *staticProp = value;
   } else {
//    obj->*memberProp = value;
   }
   throw std::runtime_error("unimplemented");
  }
 };

 class JClass: public JObject, _jclass {
 protected:
  AllocStack<JMethodID *> functions;
  AllocStack<JFieldID *> fields;

  bool isNameCompliant(const char *name) {
   //TODO
   return true;
  }

 public:
  const char * const name;

  JClass(const char * name):
   JObject(this),
   name(name)
  {}

  virtual JObject * newInstance(va_list args) = 0;
 };

 //This segment must be below the base definition for JClass
 namespace _CX {template<typename>
  class ArrayTypeBase;

  DEFINE_ARRAY_TYPE(_jbooleanArray, JBoolean)
  DEFINE_ARRAY_TYPE(_jbyteArray, JByte)
  DEFINE_ARRAY_TYPE(_jcharArray, JChar)
  DEFINE_ARRAY_TYPE(_jshortArray, JShort)
  DEFINE_ARRAY_TYPE(_jintArray, JInt)
  DEFINE_ARRAY_TYPE(_jfloatArray, JFloat)
  DEFINE_ARRAY_TYPE(_jlongArray, JLong)
  DEFINE_ARRAY_TYPE(_jdoubleArray, JDouble)
//  DEFINE_ARRAY_TYPE(_jobjectArray, JObject)
 }

//TODO finish
template<typename T>
class NativeObject: public JClass {
public:
 //TODO filter args into groups for functions
 // Members, Static members(or non-members), Member Functions, Static Functions(or non-member functions)
 //Initialization should be in pairs
 template<typename... Args>
 NativeObject(Args... args): JClass(T::name) {
  static_assert(std::is_base_of<NativeObject, T>::value, "T must be derived from NativeObject<T>!");
  static_assert(std::is_default_constructible<T>::value, "All native objects must be default constructable!");
 }

 virtual ~NativeObject() = default;

 template<typename M>
 bool registerMember(M T::*mem) {
  static_assert(_CX::JniTypeBase<M>::isRegisteredType, "Registered members must be valid JNI types!");
  throw std::runtime_error("unimplemented");
 }

 template<typename M>
 bool registerMember(M *mem) {
//  static_assert();
  throw std::runtime_error("unimplemented");
 }

 //Member methods
 template<typename R, typename ...Args>
 bool registerMethod(R (T::*func)(Args...)) {
  static_assert(_CX::JniTypeBase<R>::isRegisteredType, "Registered functions must return a valid JNI type!");
  static_assert(
   (_CX::JniTypeBase<Args>::isRegisteredType && ...),
   "Registered functions can only accept valid JNI types!"
  );
  throw std::runtime_error("unimplemented");
 }

 //Non-member or static methods
 template<typename R, typename...Args>
 bool registerMethod(R (*func)(Args...)) {
  throw std::runtime_error("Unimplemented");
 }

 JObject * newInstance(va_list args) override {
  return (JObject *)new T();
 }
};

 //TODO
// class JWeak: JObject {
// public:
//  explicit JWeak(const std::string name): JObject() {
//
//  }
// };

 template<typename>
 class Array;

 using JBooleanArray = Array<_jbooleanArray>;
 using JByteArray = Array<_jbyteArray>;
 using JCharArray = Array<_jcharArray>;
 using JShortArray = Array<_jshortArray>;
 using JIntArray = Array<_jintArray>;
 using JFloatArray = Array<_jfloatArray>;
 using JLongArray = Array<_jlongArray>;
 using JDoubleArray = Array<_jdoubleArray>;
 using JObjectArray = Array<_jobjectArray>;

 template<typename T>
 class Array: public T, JObject {
 private:
  using meta = _CX::ArrayTypeBase<T>;

 public:
  using componentType = typename meta::type;
  const uint32_t size;
  componentType * const array;

  explicit Array(const Array<T> &array) = delete;

  explicit Array(const uint32_t size): Array(size, meta::name) {}

  explicit Array(const uint32_t size, const std::string name):
   T(),
   //TODO Are all non-primitive, and multidimensional, arrays just object arrays?
//   JObject(_CX::JniTypeBase<JObjectArray>::signature),
//   JObject(_CX::JniTypeBase<T>::clazz),
   JObject(nullptr),
   size(size),
   array(new componentType[size])
  {
   static_assert(std::is_base_of<JArray, T>::value, "T must be derived from _jarray!");
  }

  virtual ~Array() {
   delete array;
  }

  componentType& operator[](const uint32_t i) {
   if (i >= size) {
    throw std::out_of_range("");
   }
   return &(array[i]);
  }
 };

 class JString: public JCharArray, _jstring {
 public:
  explicit JString(const uint32_t size): Array(size, "JString") {}

  explicit JString(const uint32_t size, const char * const str): JString(size) {
   //Strings are not null terminated in the jvm
   if (size > strlen(str)) {
    throw std::out_of_range("Supplied is greater than the supplied string!");
   }
   memcpy(array, str, size);
  }
 };
}