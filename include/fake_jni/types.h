#pragma once

#include "jni.h"

#include "util.h"

#include <string>
#include <mutex>

#ifdef _WIN32
#define STATIC_LIB_SUFFIX std::string(".lib")
#define DYNAMIC_LIB_SUFFIX std::string(".dll")
#else
#define STATIC_LIB_SUFFIX std::string(".a")
#define DYNAMIC_LIB_SUFFIX std::string(".so")
#endif

namespace FakeJni {
 using dlopen_t = void* (* const)(const char *filename, int flags);
 using dlsym_t = void* (* const)(void *handle, const char* symbol);
 using dlclose_t = int (* const)(void* handle);

 using JBoolean = jboolean;
 using JByte = jbyte;
 using JChar = jchar;
 using JShort = jshort;
 using JInt = jint;
 using JFloat = jfloat;
 using JLong = jlong;
 using JDouble = jdouble;

// using JObject = _jobject;
 class JObject;
 class JClass;
 using JThrowable = _jthrowable;
 using JArray = _jarray;

 class Jvm;

 class InvokeInterface;
 class NativeInterface;

 class Library;
 class LibraryOptions;

 class JniEnv: public JNIEnv {
 public:
  Jvm * const vm;

 public:
  JniEnv(Jvm * const vm):
   JNIEnv(),
   vm(vm)
  {}

  virtual Jvm * getVM() final {
   return vm;
  }
 };

 class Jvm: public JavaVM {
 protected:
  inline static AllocStack<Jvm *> vms;

  //TODO use proper UUID format in the future
  inline static char * generateJvmUuid() {
   static auto randCharFunc = []() -> char {
    return (char)((rand() % 2 ? 65 : 97) + (rand() % 25));
   };
   auto str = new char[32];
   for (uint32_t i = 0; i < 32; i++) {
    str[i] = randCharFunc();
   }
   //Ensure that the uuid is unique
   bool changed = true;
   uint32_t i = 0;
   while (changed && i < vms.getSize()) {
    changed = false;
    while(vms[i]->uuid == std::string(str)) {
     str[rand() % 31] = randCharFunc();
     changed = true;
    }
    i = (changed ? 0 : i + 1);
   }
   return str;
  }

 public:
  const char * uuid;

  explicit Jvm(): JavaVM(), uuid(generateJvmUuid()) {
   vms.pushAlloc(this);
  }

  virtual ~Jvm() {
   vms.removeAlloc(this);
   delete uuid;
  }

  //Unstable api
  virtual InvokeInterface * getInvokeInterface() = 0;
  virtual NativeInterface * getNativeInterface() = 0;
  //Stable api
  virtual FILE * getLog() = 0;
  virtual JniEnv * getEnv() = 0;
  virtual void destroy() = 0;
  virtual Library * getLibrary(const std::string &path) = 0;
  virtual Library * attachLibrary(
   const std::string &path,
   const std::string &options,
   LibraryOptions loptions
  ) = 0;
  virtual bool removeLibrary(const std::string &path, const std::string &options) = 0;
 };
}