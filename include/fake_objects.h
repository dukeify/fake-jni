#include <assert.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <jni.h>

#pragma once

template<typename R, typename... A>
using FuncRef = R (*)(A...);

template<typename R, typename T, typename... A>
using MemberFuncRef = R (T::*)(A...);

class FakeClass;

class _jfieldID {
private:
  inline static std::vector<jfieldID> fields;

  void assureUnique(_jfieldID* const fid) {
    if (getFieldID(*fid)) {
      throw std::runtime_error("This field is already registered!");
    }
    fields.push_back(fid);
  }

public:
  const bool isStatic;
  const std::string name, sig;
  union {
    void* const sField;
    // member field pointers cannot be declared as void?
    //  void FakeClass::* const field;
    int FakeClass::*const field;
  };

  template<typename F>
  explicit _jfieldID(std::string name, F* field) :
      // isStatic(true), name(std::move(name)), sig(generateSignature(field)),
      // sField(field) { safety third
      isStatic(true), name(std::move(name)), sig(generateSignature(field)),
      sField((void* const) field) {
    fields.push_back(this);
  }

  template<typename F, typename D>
  explicit _jfieldID(std::string name, F D::*field) :
      // isStatic(false), name(std::move(name)),
      // sig(generateSignature(field)), field(static_cast<int FakeClass::*
      // const>(field)) { safety third
      isStatic(false), name(std::move(name)), sig(generateSignature(field)),
      field((int FakeClass::*const) field) {
    fields.push_back(this);
  }

  // TODO problem also here, the returned type needs to be stored in the case of
  // instance members in order to
  template<typename F>
  F* const getField(FakeClass* const clazz) {
    if (isStatic) {
      return reinterpret_cast<F* const>(sField);
    } else {
      //   return clazz->*reinterpret_cast<F FakeClass::* const>(field);
      return nullptr;
    }
  }

  template<typename F>
  void setField(FakeClass* clazz, F value) {
    if (isStatic) {
      *reinterpret_cast<F* const>(sField) = value;
    } else {
      (clazz->*reinterpret_cast<F FakeClass::*const>(field)) = value;
    }
  }

  bool operator==(const _jfieldID& fid) const {
    if (fid.isStatic == isStatic) {
      return (isStatic ? fid.sField == sField : fid.field == field);
    }
    return false;
  }

  // TODO merge generateSignature function overloads into 1
  template<typename F>
  static std::string generateSignature(F* field);

  template<typename F, typename D>
  static std::string generateSignature(F D::*const field);

  static jfieldID getFieldID(const _jfieldID& fid) {
    for (const auto& jfid : fields) {
      if (*jfid == fid) {
        return jfid;
      }
    }
    return nullptr;
  }

  // May produce collisions, see notes on FakeClass
  static jfieldID getFieldID(std::string name, std::string sig) {
    for (const auto& fid : fields) {
      if (name == fid->name && sig == fid->sig) {
        return fid;
      }
    }
    return nullptr;
  }
};

// TODO implement signature generation like _jfieldID
class _jmethodID {
private:
  inline static std::vector<jmethodID> methods;

  static void assureUnique(_jmethodID* const mid) {
    if (getMethodID(*mid)) {
      throw std::runtime_error("This method is already registered!");
    }
    methods.push_back(mid);
  }

public:
  // TODO
  // const std::string name, sig;
  const bool isStatic;

  union {
    void (*const sFunc)();
    void (FakeClass::*const func)();
  };

  template<typename R, typename... A>
  explicit _jmethodID(FuncRef<R, A...> func) :
      // isStatic(true), sFunc(static_cast<void (* const)()>(func)) {
      // safety third
      isStatic(true), sFunc((void (*const)()) func) {
    assureUnique(this);
  }

  template<typename R, typename D, typename... A>
  explicit _jmethodID(MemberFuncRef<R, D, A...> func) :
      // isStatic(false), func(static_cast<void (FakeClass::* const)()>(func))
      // { safety third
      isStatic(false), func((void (FakeClass::*const)()) func) {
    assureUnique(this);
  }

  // TODO problem lies here, the return type needs to be R * in order to work
  // with the JNI... how should returned values
  // be stored?
  template<typename R, typename... A>
  R invoke(FakeClass* const clazz, A... args) {
    if (isStatic) {
      return reinterpret_cast<FuncRef<R, A...>>(sFunc)(args...);
    } else {
      return (clazz->*reinterpret_cast<MemberFuncRef<R, FakeClass, A...>>(
                          func))(args...);
    }
  }

  bool operator==(const _jmethodID& rhs) const {
    if (rhs.isStatic == isStatic) {
      return (isStatic ? rhs.sFunc == sFunc : rhs.func == func);
    }
    return false;
  }

  // _jmethodID operator =(const _jmethodID &mid) {
  //  return *this;
  // }

  static jmethodID getMethodID(const _jmethodID& mid) {
    for (const auto& jmid : methods) {
      if (*jmid == mid) {
        return jmid;
      }
    }
    return nullptr;
  }

  // TODO
  static jmethodID getMethodID(std::string name, std::string sig) {
    return nullptr;
  }

  // TODO
  template<typename R, typename... A>
  std::string generateSignature(FuncRef<R, A...> func);

  // TODO
  template<typename R, typename D, typename... A>
  std::string generateSignature(MemberFuncRef<R, D, A...> func);
};

// NOTE Because of the singly linked ascending class hierarchy, there is no
// prevention against linking single methods
// or fields to multiple classes. This could lead to collisions where two
// separate classes define identical fields and the incorrect field is resolved
// on a depth-first-search. Sticking to the high-level API will prevent
// collisions, however, using the JNI will not. This needs to be fixed
// eventually! Requires a classloader implementation.
//
// NOTE Using references prevents users from having to allocate memory to create
// _jmethodIDs or _jfieldIDs. To simplify
// interactions with the JNI, the typedef'd pointer equivalents of classes are
// used internally.
// TODO implement instance differentiation (classloader required)
class FakeClass : public _jclass {
private:
  // Global map of registered classes
  inline static std::vector<FakeClass*> classes;

  std::vector<jmethodID> methods;
  std::vector<jfieldID> fields;

public:
  const std::string fullyQualifiedName;

  explicit FakeClass(std::string fullyQualifiedName) :
      fullyQualifiedName(std::move(fullyQualifiedName)) {
    registerClass(this);
  }

  virtual ~FakeClass() = default;

  bool isMethodRegistered(const _jmethodID& mid) {
    for (const auto& m : methods) {
      if (*m == mid) {
        return true;
      }
    }
    return false;
  }

  // TODO should errors here be fatal?
  void registerMethod(const _jmethodID& mid) {
    if (isMethodRegistered(mid)) {
      throw std::runtime_error("That method is already bound to this class.");
    }
    methods.push_back(_jmethodID::getMethodID(mid));
  }

  // Handles both instance and static methods
  template<typename R, typename... A>
  // R invokeMethod(const _jmethodID &mid, A... args) {
  R invokeMethod(_jmethodID mid, A... args) {
    return mid.invoke<R, A...>(this, args...);
  }

  bool isFieldRegistered(const _jfieldID& fid) {
    for (const auto& f : fields) {
      if (*f == fid) {
        return true;
      }
    }
    return false;
  }

  // TODO should errors here be fatal?
  void registerField(const _jfieldID& fid) {
    if (isFieldRegistered(fid)) {
      throw std::runtime_error("That field is already bound to this class.");
    }
    fields.push_back(_jfieldID::getFieldID(fid));
  }

  template<typename F>
  // F * const getField(const _jfieldID &fid) {
  F* const getField(_jfieldID& fid) {
    if (!isFieldRegistered(fid)) {
      throw std::runtime_error("That field is not bound to this class!");
    }
    return fid.getField<F>(this);
  }

  template<typename F>
  // void setField(const _jfieldID &fid, F * const value) {
  void setField(_jfieldID& fid, F value) {
    if (!isFieldRegistered(fid)) {
      throw std::runtime_error("That field is not bound to this class!");
    }
    fid.setField(this, value);
  }

  static jclass getClass(const std::string& classpath) {
    for (const auto& clazz : classes) {
      if (clazz->fullyQualifiedName == classpath) {
        return clazz;
      }
    }
    return nullptr;
  }

  static bool isClassRegistered(FakeClass* const fakeClass) {
    for (const auto& clazz : classes) {
      if (clazz == fakeClass) {
        return true;
      }
    }
    return false;
  }

  // TODO should errors here be fatal?
  static void registerClass(FakeClass* fakeClass) {
    if (isClassRegistered(fakeClass)) {
      throw std::runtime_error(
          "This class has already been registered on the JVM!");
    }
    classes.push_back(fakeClass);
  }
};

// TODO finish signature cases
template<typename F>
std::string _jfieldID::generateSignature(F* field) {
  std::string sig;
  if constexpr (std::is_base_of<FakeClass, F>::value) {
    sig = "L" + reinterpret_cast<FakeClass*>(field)->fullyQualifiedName;
  } else if constexpr (std::is_same<F, jboolean>::value) {
    sig = "Z";
  } else if constexpr (std::is_same<F, jbooleanArray>::value) {
    sig = "[Z";
  }
  return sig + ";";
}

// TODO finish
template<typename F, typename D>
std::string _jfieldID::generateSignature(F D::*const field) {
  return "";
}

// JNI Functions
extern "C"
{
  jclass GetObjectClass(JNIEnv* env, jobject obj) {
    assert(FakeClass::isClassRegistered(static_cast<FakeClass*>(obj)));
    // TODO how would you determine whether or not 'obj' is an instance of
    // FakeClass without templates or virtual functions?
    //  jclass clazz = dynamic_cast<jclass>(obj);
    //  assert(!clazz && "The jobject must inherit from FakeClass!");
    return (FakeClass*) obj;
  }

  // Since no classloader model has been implemented, the current implementation
  // of this function does not require the target class to resolved
  jmethodID GetStaticMethodID(JNIEnv* env,
                              jclass clazz,
                              const char* name,
                              const char* sig) {
    jmethodID const mid = _jmethodID::getMethodID(name, sig);
    if (mid == nullptr || !mid->isStatic) {
      // TODO error is placeholder
      throw std::runtime_error("NoSuchMethodError");
    }
    return mid;
  }

  // TODO waiting on return stack implementation
  jobject CallObjectMethod(jobject obj, jmethodID mid, ...) {
    va_list args;
    ((FakeClass*) obj)->invokeMethod<_jobject>(*mid, args);
    return nullptr;
  }

  // Since no classloader model has been implemented, the current implementation
  // of this function does not require the target class to resolved
  jfieldID
  GetFieldID(JNIEnv* env, jclass clazz, const char* name, const char* sig) {
    //  FakeClass *fakeClass = static_cast<FakeClass *>(clazz);
    //  if (!fakeClass) {
    //   //TOODO error is placeholder
    //   throw std::runtime_error("Invalid class instance pointer!");
    //   //return nullptr;
    //  }
    jfieldID const fid = _jfieldID::getFieldID(name, sig);
    if (!fid) {
      throw std::runtime_error("NoSuchFieldError");
    }
    return fid;
  }

  jobject GetObjectField(JNIEnv* env, jobject obj, jfieldID fid) {
    // template parameters cannot be deduced for whatever reason
    return fid->getField<_jobject>((FakeClass*) obj);
  }

  jboolean GetBooleanField(JNIEnv* env, jobject obj, jfieldID fid) {
    return *((FakeClass*) obj)->getField<jboolean>(*fid);
  }

  jbyte GetByteField(JNIEnv* env, jobject obj, jfieldID fid) {
    return *((FakeClass*) obj)->getField<jbyte>(*fid);
  }

  jchar GetCharField(JNIEnv* env, jobject obj, jfieldID fid) {
    return *((FakeClass*) obj)->getField<jchar>(*fid);
  }

  jshort GetShortField(JNIEnv* env, jobject obj, jfieldID fid) {
    return *((FakeClass*) obj)->getField<jshort>(*fid);
  }

  jint GetIntField(JNIEnv* env, jobject obj, jfieldID fid) {
    return *((FakeClass*) obj)->getField<jint>(*fid);
  }

  jlong GetLongField(JNIEnv* env, jobject obj, jfieldID fid) {
    return *((FakeClass*) obj)->getField<jlong>(*fid);
  }

  jfloat GetFloatField(JNIEnv* env, jobject obj, jfieldID fid) {
    return *((FakeClass*) obj)->getField<jfloat>(*fid);
  }

  jdouble GetDoubleField(JNIEnv* env, jobject obj, jfieldID fid) {
    return *((FakeClass*) obj)->getField<jdouble>(*fid);
  }

  void SetObjectField(JNIEnv* env, jobject obj, jfieldID fid, jobject value) {
    fid->setField((FakeClass*) obj, *value);
  }

  void SetBooleanField(JNIEnv* env, jobject obj, jfieldID fid, jboolean value) {
    fid->setField((FakeClass*) obj, value);
  }

  void SetByteField(JNIEnv* env, jobject obj, jfieldID fid, jbyte value) {
    fid->setField((FakeClass*) obj, value);
  }

  void SetCharField(JNIEnv* env, jobject obj, jfieldID fid, jchar value) {
    fid->setField((FakeClass*) obj, value);
  }

  void SetShortField(JNIEnv* env, jobject obj, jfieldID fid, jshort value) {
    fid->setField((FakeClass*) obj, value);
  }

  void SetIntField(JNIEnv* env, jobject obj, jfieldID fid, jint value) {
    fid->setField((FakeClass*) obj, value);
  }

  void SetLongField(JNIEnv* env, jobject obj, jfieldID fid, jlong value) {
    fid->setField((FakeClass*) obj, value);
  }

  void SetFloatField(JNIEnv* env, jobject obj, jfieldID fid, jfloat value) {
    fid->setField((FakeClass*) obj, value);
  }

  void SetDoubleField(JNIEnv* env, jobject obj, jfieldID fid, jdouble value) {
    fid->setField((FakeClass*) obj, value);
  }
}