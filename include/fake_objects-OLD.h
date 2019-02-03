//#include <jni.h>
//#include <utility>
//#include <string>
//#include <vector>
//#include <map>
////#include <set>
//#include <assert.h>
//#include <iostream>
//
//#pragma once
//
//template<typename R, typename... A>
//using FuncRef = R (*)(A...);
//
//template<typename R, typename T, typename... A>
//using MemberFuncRef = R (T::*)(A...);
//
//class FakeClass : public _jclass {
//private:
// //Global map of registered classes
// inline static std::map<const std::string, FakeClass *> classes;
// //Global map of static functions
// //inline static std::map<FakeClass *, std::vector<std::pair<_jmethodID, void (*)()>>> staticFunctionTable;
//
// std::map<jmethodID, MemberFuncRef<void, FakeClass>> memberFunctionTable;
// std::map<jmethodID, FuncRef<void>> staticFunctionTable;
// //TODO Void type member pointers are disallowed?
//// std::map<jfieldID, void FakeClass::*> memberFields;
// std::map<jfieldID, int FakeClass::*> memberFields;
// std::map<jfieldID, void *> staticFields;
//
//public:
// explicit FakeClass(const std::string &fullyQualifiedName) {
//  assert(registerClass(fullyQualifiedName, this));
// }
//
// virtual ~FakeClass() = default;
//
// std::string getFullyQualifiedName();
//
// template<typename R, typename... A>
// void registerMethod(jmethodID mid, FuncRef<R, A...> func);
//
// template<typename R, typename... A>
// void registerMethod(jmethodID mid, MemberFuncRef<R, FakeClass, A...> func);
//
// bool isMethodRegistered(jmethodID mid);
//
// template<typename... A>
// _jobject invokeMethod(jmethodID mid, A... args);
//
// template<typename... A>
// _jobject invokeStaticMethod(jmethodID mid, A... args);
//
// template<typename F>
// void registerField(const _jfieldID &fid, F *field);
//
// template<typename F>
// void registerField(const _jfieldID &fid, F FakeClass::* field);
//
//// template<typename F>
//// bool isFieldRegistered(const _jfieldID &fid);
//
// jfieldID getFieldID(const char *name, const char *sig);
//
// template<typename F>
// F * getField(jfieldID fid);
//
// template<typename F>
// void setField(jfieldID fid, const F &value);
//
// static bool registerClass(const std::string &fqn, FakeClass *fakeClass);
//
// static bool isClassRegistered(FakeClass *fakeClass);
//
// static jclass getClass(const std::string &classpath);
//};
//
//class _jfieldID {
//private:
// inline static std::vector<jfieldID> fields;
//
//public:
// const std::string name, sig;
//
// explicit _jfieldID(std::string name, std::string sig) : name(std::move(name)), sig(std::move(sig)) {
//  fields.push_back(this);
// }
//
// bool operator ==(const _jfieldID &rhs) const;
//
// template<typename A>
// static _jfieldID generateFieldID(const std::string &name, A *field);
//
// static jfieldID getFieldInstance(const _jfieldID &fid);
//};
//
////dummy data structure to satisfy JNI
//class _jmethodID {
//private:
// inline static std::vector<jmethodID> methods;
//
//// template<typename R, typename... A>
//// _jmethodID(std::string name, FuncRef<R, A...> func) : name(std::move(name)), func(static_cast<void (*)()>(func)) {
////  instances.push_back(this);
//// }
//
//public:
// _jmethodID() {
//  methods.push_back(this);
// }
//// const std::string name;
//// void (* const func)();
//
// //bool operator <(const _jmethodID &rhs) const;
//
// //bool operator ==(const _jmethodID &rhs);
//
//// static jmethodID getMethodInstance(const _jmethodID &mid) {
////  for (const auto &m : instances) {
////   if (*m == mid) {
////    return m;
////   }
////  }
////  return nullptr;
//// }
//};
//
////BEGIN FakeClass DEFINITIONS
//
//std::string FakeClass::getFullyQualifiedName() {
// std::string name;
// for (const auto &[key, value] : classes) {
//  if (value == this) {
//   name = key;
//  }
// }
// return name;
//}
//
//template<typename R, typename... A>
//void FakeClass::registerMethod(jmethodID mid, FuncRef<R, A...> func) {
// static_assert(
//  std::is_base_of<R, _jobject>::value || std::is_base_of<R, void>::value,
//  "The function's return type must inherit from _jobject!"
// );
// if (isMethodRegistered(mid)) {
//  throw std::runtime_error("This function is already registered!");
// } else {
//  staticFunctionTable[mid] = reinterpret_cast<void (*)()>(func);
// }
//}
//
//template<typename R, typename... A>
//void FakeClass::registerMethod(jmethodID mid, MemberFuncRef<R, FakeClass, A...> func) {
//
//}
//
//bool FakeClass::isMethodRegistered(jmethodID mid) {
//// return memberFunctionTable[mid] != nullptr || staticFunctionTable[mid] != nullptr;
// return memberFunctionTable[mid] || staticFunctionTable[mid];
//}
//
//template<typename... A>
//_jobject FakeClass::invokeStaticMethod(jmethodID mid, A... args) {
// return reinterpret_cast<FuncRef<_jobject, A...>>(staticFunctionTable[mid])(args...);
//}
//
//template<typename... A>
//_jobject FakeClass::invokeMethod(jmethodID mid, A... args) {
// return reinterpret_cast<MemberFuncRef<_jobject, FakeClass, A...>>(memberFunctionTable[mid])(args...);
//}
//
//template<typename F>
//void FakeClass::registerField(const _jfieldID &fid, F *field) {
// static_assert(std::is_base_of<F, _jobject>::value, "Fields must inherit from _jobject!");
// jfieldID f = _jfieldID::getFieldInstance(fid);
// if (f != nullptr) {
//  throw std::runtime_error("This field is already registered!");
// } else {
//  memberFields[f] = field;
// }
//}
//
////template<typename F>
////bool FakeClass::isFieldRegistered(const _jfieldID &fid) {
//// return (_jfieldID::getFieldInstance(fid)) != nullptr;
////// for (const auto &[key, value]: fields) {
//////  if (*key == fid && value == field) {
//////   return true;
//////  }
////// }
////// return false;
////}
//
//jfieldID FakeClass::getFieldID(const char *name, const char *sig) {
// const std::string
//  sname = std::string(name),
//  ssig = std::string(sig);
// for (const auto &[key, value] : memberFields) {
//  if (key->name == sname && key->sig == ssig) {
//   return key;
//  }
// }
// return nullptr;
//}
//
//template<typename F>
//F * FakeClass::getField(jfieldID fid) {
// return static_cast<F *>(memberFields[fid]);
//}
//
//template<typename F>
//void FakeClass::setField(jfieldID fid, const F &value) {
// memberFields[fid] = value;
//}
//
//bool FakeClass::registerClass(const std::string &fqn, FakeClass *fakeClass) {
// if (!isClassRegistered(fakeClass)) {
//  classes[fqn] = fakeClass;
//  return true;
// }
// return false;
//}
//
//bool FakeClass::isClassRegistered(FakeClass *fakeClass) {
// for (const auto &[key, value]: classes) {
//  if (value == fakeClass) {
//   return true;
//  }
// }
// return false;
//}
//
//jclass FakeClass::getClass(const std::string &classpath) {
// for (auto const &[key, value]: classes) {
//  if (key == classpath) {
//   return value;
//  }
// }
// return nullptr;
//}
//
//bool _jfieldID::operator==(const _jfieldID &rhs) const {
// return name == rhs.name && sig == rhs.sig;
//}
//
//template<typename A>
//_jfieldID _jfieldID::generateFieldID(const std::string &name, A *field) {
// std::string sig;
// //TODO all other type specifiers
// if constexpr(std::is_base_of<FakeClass, A>::value) {
//  sig = "L" + static_cast<FakeClass *>(field)->getFullyQualifiedName();
// }
///*  else constexpr () {
//
//  }*/
// return _jfieldID(name, sig);
//}
//
//jfieldID _jfieldID::getFieldInstance(const _jfieldID &fid) {
// for (const auto &f : fields) {
//  if (*f == fid) {
//   return f;
//  }
// }
// return nullptr;
//}
//
////bool _jmethodID::operator <(const _jmethodID &rhs) const {
//// return instVal < rhs.instVal;
////}
//
////JNI Function Implementations
//extern "C" {
// jclass GetObjectClass(JNIEnv *env, jobject obj) {
//  assert(FakeClass::isClassRegistered(static_cast<FakeClass *>(obj)));
//  //TODO how would you determine whether or not 'obj' is an instance of
//  // FakeClass without templates or virtual functions?
////  jclass clazz = dynamic_cast<jclass>(obj);
////  assert(!clazz && "The jobject must inherit from FakeClass!");
//  return (jclass)obj;
// }
//
// jmethodID GetStaticMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
//  return nullptr;
// }
//
// //TODO Requires a stack to store returned values
// jobject CallObjectMethod(jobject obj, jmethodID mid, ...) {
//  //static_assert<std::is_base_of<>::value, "The jobject must inherit from FakeClass!">
//  /*
//   FakeClass *fakeClass = dynamic_cast<FakeClass *>(obj);
//   if (fakeClass == nullptr) {
//    assert(false && "The jobject must inherit from FakeClass!");
//   }
//  */
//  va_list args;
//  //TODO Is there a safer way to do this???
//  auto fakeClass = (FakeClass *)obj;
//  //static _jobject returned = fakeClass->invokeStaticMethod(*mid, args);
//  //return &returned;
//  fakeClass->invokeStaticMethod(mid, args);
//  //TODO FIX!
//  return nullptr;
// }
//
// /*
// //TODO
// jfieldID GetFieldID(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
//  FakeClass *fakeClass = static_cast<FakeClass *>(clazz);
//  if (!fakeClass) {
//   //TODO error is placeholder
//   throw std::runtime_error("Invalid class instance pointer!");
//   //return nullprt;
//  }
//  jfieldID fid = fakeClass->getFieldID(name, sig);
//  if (!fid) {
//   throw std::runtime_error("NoSuchFieldError");
//  }
//  return fid;
// }
//
// jobject GetObjectField(JNIEnv *env, jobject obj, jfieldID fid) {
//  //template parameters cannot be deduced for whatever reason
//  return static_cast<FakeClass *>(obj)->getField<_jobject>(fid);
// }
//
// jboolean GetBooleanField(JNIEnv *env, jobject obj, jfieldID fid) {
////  return static_cast<jboolean>(GetObjectField(env, obj, fid));
// }
//
// jbyte GetByteField(JNIEnv *env, jobject obj, jfieldID fid) {
////  return static_cast<jbyte>(GetObjectField(env, obj, fid));
// }
//
// jchar GetCharField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// jshort GetShortField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// jint GetIntField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// jlong GetLongField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// jfloat GetFloatField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// jdouble GetDoubleField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// void SetObjectField(JNIEnv *env, jobject obj, jfieldID fid, jobject value) {
//  const FakeClass *fakeClass = static_cast<FakeClass *>(obj);
//  auto ret = fakeClass->SetField(fid, value);
// }
//
// void SetBooleanField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// void SetByteField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// void SetCharField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// void SetShortField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// void SetIntField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// void SetLongField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// void SetFloatField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//
// void SetDoubleField(JNIEnv *env, jobject obj, jfieldID fid) {
//
// }
//  */
//}
//
