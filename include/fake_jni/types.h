#pragma once

#include "jni.h"

#include <string>
#include <mutex>

#ifdef _WIN32
#define STATIC_LIB_SUFFIX std::string(".lib")
#define DYNAMIC_LIB_SUFFIX std::string(".dll")
#else
#define STATIC_LIB_SUFFIX std::string(".a")
#define DYNAMIC_LIB_SUFFIX std::string(".so")
#endif

namespace FakeJVM {
 using dlopen_t = void* (* const)(const char *filename, int flags);
 using dlsym_t = void* (* const)(void *handle, const char* symbol);
 using dlclose_t = int (* const)(void* handle);

 using JniEnv = JNIEnv;

 class Jvm;

 class InvokeInterface;
 class NativeInterface;

 class Library;
 class LibraryOptions;

 template<typename T>
 class List {
 public:
  T t;
  List<T> *next;
 };

 class Jvm: public JavaVM {
 protected:
  inline static List<Jvm *> * vms = nullptr;
  inline static std::mutex vms_mutex;

  //TODO use proper UUID format in the future
  inline static char * generateJvmUuid() {
   static auto randCharFunc = []() -> char {
    return (char)((rand() % 2 ? 65 : 97) + (rand() % 25));
   };
   auto str = new char[32];
   for (uint32_t i = 0; i < 32; i++) {
    str[i] = randCharFunc();
   }
   List<Jvm *> * vm_p = vms;
   while(vm_p != nullptr) {
    bool changed = false;
    while(vm_p->t->uuid == std::string(str)) {
     str[rand() % 31] = randCharFunc();
     changed = true;
    }
    vm_p = changed ? vms : vm_p->next;
   }
   return str;
  }

 public:
  const char * uuid;

  explicit Jvm():
   JavaVM(),
   uuid(generateJvmUuid())
  {
   //Add new Jvm instance to the global Jvm list
   std::scoped_lock<std::mutex> vms_lock(vms_mutex);
   const auto inst = new List<Jvm *> { this, nullptr };
   if (vms == nullptr) {
    vms = inst;
   } else {
    List<Jvm *> *vm = vms;
    while (vm->next != nullptr) {
     vm = vm->next;
    }
    vm->next = inst;
   }
  }

  ~Jvm() {
   //Remove Jvm instance from the global Jvm list
   std::scoped_lock<std::mutex> vms_lock(vms_mutex);
   List<Jvm *>
    *vm_p = vms,
    *prev = nullptr;
   while (vm_p->t != this) {
    prev = vm_p;
    vm_p = vm_p->next;
   }
   if (prev != nullptr && vm_p->next != nullptr) {
    prev->next = vm_p->next;
   }
   delete vm_p;
   delete uuid;
  }

  //Unstable api
  virtual InvokeInterface getInvokeInterface() = 0;
  virtual NativeInterface getNativeInterface() = 0;
  //Stable api
  virtual FILE * getLog() = 0;
  virtual JNIEnv * getEnv() = 0;
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