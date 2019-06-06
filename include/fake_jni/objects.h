#include <utility>

#include <utility>

#pragma once

#include <cstring>

//TODO JClass definitions
#define DEFINE_ARRAY_TYPE(arrayType, componentType) \
template<>\
class TypeSelectorBase<arrayType> {\
public:\
 using type = componentType;\
 const char * const name = #arrayType;\
};

#define DEFINE_JNI_TYPE(type, sig) \
template<>\
class JniTypeBase<type> {\
public:\
 const bool isRegisteredType = true;\
 const char * const signature = #sig;\
};

namespace FakeJni {

 struct _jmethodID {};
 struct _jfieldID {};

 //TODO
 class JMethodID: public _jmethodID, private JNINativeMethod {
 private:
  //TODO finish breeder and impls
  template<typename F>
  inline static const char * generateSignature(F func) {
   throw std::runtime_error("unimplemented");
  }

  union {
   void (* const staticFunc)();
   void (JObject::* const memberFunc)();
  };

  explicit JMethodID(char * const name, char * const signature, const bool isStatic):
   _jmethodID(),
   JNINativeMethod { name, signature, nullptr },
   isStatic(isStatic)
  {}

 public:
  const bool isStatic;

  //that's not right
  template<typename T>
  JMethodID(const std::string name, void (T::* const func)()):
   JMethodID(name, generateSignature(func), true)
  {
   static_assert(std::is_member_function_pointer<T>::value, "Argument is not a member function!");
  }

  template<typename F>
  JMethodID(const std::string name, F * const func):
   JMethodID(name, generateSignature(func), true)
  {
   static_assert(std::is_function<F>::value, "Argument is not a function!");
  }
 };

 //TODO
 class JFieldID: public _jfieldID {
 public:
  JFieldID(): _jfieldID() {

  }
 };

 class JObject: public _jobject {
 public:
  JClass * const clazz;

  explicit JObject(JClass * const clazz):
   clazz(clazz)
  {}
 };

 class JClass: public JObject, _jclass {
 protected:
  AllocStack<JMethodID *> functions;
  AllocStack<JFieldID *> fields;

  //TODO
  void assertNameCompliance(const std::string &name) {

  }

 public:
  const std::string name;

  explicit JClass(): JObject(this) {

  }
 };

 //Stowaway namespace for template junk
 namespace _CX {
  template<typename>
  class TypeSelectorBase;

  DEFINE_ARRAY_TYPE(_jbooleanArray, JBoolean)
  DEFINE_ARRAY_TYPE(_jbyteArray, JByte)
  DEFINE_ARRAY_TYPE(_jcharArray, JChar)
  DEFINE_ARRAY_TYPE(_jshortArray, JShort)
  DEFINE_ARRAY_TYPE(_jintArray, JInt)
  DEFINE_ARRAY_TYPE(_jfloatArray, JFloat)
  DEFINE_ARRAY_TYPE(_jlongArray, JLong)
  DEFINE_ARRAY_TYPE(_jdoubleArray, JDouble)
  DEFINE_ARRAY_TYPE(_jobjectArray, JObject)

  template<typename T>
  class JniTypeBase {
  public:
   const bool isRegisteredType = false;
  };

  DEFINE_JNI_TYPE(JBoolean, Z)
  DEFINE_JNI_TYPE(JByte, B)
  DEFINE_JNI_TYPE(JChar, C)
  DEFINE_JNI_TYPE(JShort, S)
  DEFINE_JNI_TYPE(JInt, I)
  DEFINE_JNI_TYPE(JFloat, F)
  DEFINE_JNI_TYPE(JLong, J)
  DEFINE_JNI_TYPE(JDouble, D)
  DEFINE_JNI_TYPE(JClass, L)
 }

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
};

// //TODO
// class JWeak: JObject {
// public:
//  explicit JWeak(const std::string name): JObject() {
//
//  }
// };

 template<typename T>
 class Array: public T, JObject {
 private:
  using meta = _CX::TypeSelectorBase<T>;

 public:
  using componentType = typename meta::type;
  const uint32_t size;
  componentType * const array;

  explicit Array(const Array<T> &array) = delete;

  explicit Array(const uint32_t size): Array(size, meta::name) {}

  explicit Array(const uint32_t size, const std::string name):
   T(),
   //TODO
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

 using JBooleanArray = Array<_jbooleanArray>;
 using JByteArray = Array<_jbyteArray>;
 using JCharArray = Array<_jcharArray>;
 using JShortArray = Array<_jshortArray>;
 using JIntArray = Array<_jintArray>;
 using JFloatArray = Array<_jfloatArray>;
 using JLongArray = Array<_jlongArray>;
 using JDoubleArray = Array<_jdoubleArray>;
 using JObjectArray = Array<_jobjectArray>;

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