#pragma once

//STL dependencies
#include <type_traits>
#include <initializer_list>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <string>
#include <stdexcept>
#include <mutex>
#include <cmath>

#include <dlfcn.h>

//CX dependencies
#include "cx/strings.h"
#include "cx/templates.h"
#include "cx/classes.h"
//JNI dependencies
#include "jni.h"
#include "jvmti.h"
//fake-jni base headers
#include "util.h"
#include "types.h"
#include "library.h"
//fake-jni JVM implementation
#include "jvm/object.h"
#include "jvm/field.h"
#include "jvm/method.h"
#include "jvm/class.h"
#include "native_object.h"
#include "jvm/exception.h"
#include "jvm/array.h"
#include "jvm/weak.h"
#include "jvm/string.h"
//JNI invoke implementations
#include "jni/invoke/invoke.h"
#include "jni/invoke/thread.h"
//JNI native implementations
#include "jni/native/native.h"
#include "jni/native/native_constructor.h"
#include "jni/native/native_inlined.h"
#include "jni/native/array.h"
#include "jni/native/buffer.h"
#include "jni/native/exception.h"
#include "jni/native/field.h"
#include "jni/native/method.h"
#include "jni/native/misc.h"
#include "jni/native/object.h"
#include "jni/native/ref.h"
#include "jni/native/reflect.h"
#include "jni/native/string.h"
//JVMTI implementations
#include "jvmti/interface.h"
#include "jvmti/interface_constructor.h"
#include "jvmti/interface_inlined.h"
#include "jvmti/thread.h"
#include "jvmti/monitor.h"
#include "jvmti/class.h"
#include "jvmti/field.h"
#include "jvmti/method.h"
#include "jvmti/misc.h"

namespace FakeJni {
 //TODO all JVM calls should be blocking until execution completes to prevent race conditions from emerging
 template<typename IInvoke, typename INative, typename IJvmti, typename Env, typename JvmtiEnv>
 class JvmImpl: public Jvm {
 private:
  FILE * const log;
  IInvoke * const invoke;
  INative * const native;
  IJvmti * const jvmti;

  const Env env;
  //TODO eventually implement hooks
  const JvmtiEnv jvmtiEnv;

  //TODO on the first invocation of any JNI, JNIEnv or JVMTI functions, set this flag to true
  bool running;

  std::vector<Library *> libraries;
  std::vector<JClass *> classes;
  //TODO if classloaders are ever implemented, this property will be handled by the ClassLoader model
  AllocStack<JObject *> instances{true};

 public:
  explicit JvmImpl(): JvmImpl(stdout) {}

  explicit JvmImpl(FILE *log):
   JvmImpl(log, new IInvoke(), new INative(), new IJvmti())
  {}

  explicit JvmImpl(FILE *log, IInvoke * const invoke, INative * const native, IJvmti * const jvmti):
   Jvm(),
   log(log),
   invoke(invoke),
   native(native),
   jvmti(jvmti),
   env(Env(this)),
   jvmtiEnv(JvmtiEnv(this)),
   running(false)
  {
   functions = (const JNIInvokeInterface_ *)invoke;
  }

  virtual ~JvmImpl() {
   instances.clear();
   for (const auto &l : libraries) {
    removeLibrary(l->path, "");
   }
   libraries.clear();
   delete jvmti;
   delete native;
   delete invoke;
  }

  FILE * getLog() override {
   return log;
  }

  InvokeInterface * getInvokeInterface() override {
   return (InvokeInterface *)invoke;
  }

  NativeInterface * getNativeInterface() override {
   return (NativeInterface *)native;
  }

  JvmtiInterface * getJvmtiInterface() override {
   return (JvmtiInterface *)jvmti;
  }

  JniEnv * getEnv() override {
   return const_cast<JniEnv *>(&env);
  }

  //TODO search through all registered native classes for one containing a main method, and invoke it
  void start() override {
   throw std::runtime_error("unimplemented");
  }

  void destroy() override {
   running = false;
  }

  AllocStack<JObject *>* getInstances() override {
   return &instances;
  }

  template<typename T>
  bool registerClass() {
   static_assert(
    std::is_base_of<JClass, T>::value,
    "Only native classes may be passed to registerClass!"
   );
   JClass *clazz = const_cast<JClass *>(&T::descriptor);
   if (std::find(classes.begin(), classes.end(), clazz) != classes.end()) {
#ifdef FAKE_JNI_DEBUG
    fprintf(
     log,
     "WARNING: Class '%s' is already registered on the JVM instance '%s'!",
     clazz->getName(),
     uuid
    );
#endif
    return false;
   } else {
    classes.push_back(clazz);
   }
   return true;
  }

  template<typename T>
  bool unregisterClass() {
   static_assert(
    std::is_base_of<JClass, T>::value,
    "Only native classes may be passed to unregisterClass!"
   );
   auto clazz = const_cast<JClass *>(&T::descriptor);
   const auto end = classes.end();
   const auto ret = std::remove(classes.begin(), classes.end(), clazz);
#ifdef FAKE_JNI_DEBUG
   fprintf(
    log,
    "WARNING: Class '%s' is not registered on the JVM instance '%s'!",
    clazz->getName(),
    uuid
   );
#endif
   return end == ret;
  }

  Library * getLibrary(const std::string &path) override {
   for (const auto &l : libraries) {
    if (l->path == path) {
     return l;
    }
   }
   return nullptr;
  }

  Library * attachLibrary(
   const std::string &rpath,
   const std::string &options,
   LibraryOptions loptions = LibraryOptions()
  ) override {
   std::string path = rpath.empty() ? "(embedded)" : rpath;
   Library * library = getLibrary(path);
   if (!library) {
    library = new Library(this, path, loptions);
#ifdef FAKE_JNI_DEBUG
    fprintf(log, "DEBUG: Created library: '%s'\nDEBUG: [%s]::JNI_OnLoad\n", path.c_str(), path.c_str());
#endif
    if (library->jniBound()) {
     if (library->jniLoad()) {
      throw std::runtime_error("FATAL: Error initializing JNI library: '" + path + "'");
     }
    }
    //Only one startup agent function is called per library instance
    if (library->agentBound()) {
#ifdef FAKE_JNI_DEBUG
     fprintf(log, "DEBUG: [%s]::%s\n", path.c_str(), (running ? "Agent_OnAttach" : "Agent_OnLoad"));
#endif
     const auto agentInitializer = (running ? &Library::agentAttach : &Library::agentLoad);
     if ((library->*agentInitializer)(const_cast<char *>(options.c_str()))) {
      throw std::runtime_error("FATAL: Error initializing agent library: '" + path + "'");
     }
    }
    libraries.push_back(library);
#ifdef FAKE_JNI_DEBUG
    fprintf(log, "DEBUG: Registered library: '%s'\n", path.c_str());
#endif
   }
#ifdef FAKE_JNI_DEBUG
   else {
    fprintf(log, "WARNING: Library '%s' is already registered on this DefaultJvm instance!\n", path.c_str());
   }
#endif
   return library;
  }

  //TODO should we provide support for unloading just the agent or jni components of a library?
  //Removes a library from the DefaultJvm instance
  //Implicitly unloads the agent within the library, if there was one
  //Implicitly unloads the jni component within the library, if there was one
  bool removeLibrary(const std::string &path, const std::string &options) override {
   Library * const library = getLibrary(path);
   if (library) {
    (void)std::remove(libraries.begin(), libraries.end(), library);
#ifdef FAKE_JNI_DEBUG
    fprintf(log, "DEBUG: Removing library: '%s'\n", path.c_str());
#endif
    if (library->jniBound()) {
     library->jniUnload();
    }
    if (library->agentBound()) {
     library->agentUnload(const_cast<char *>(options.c_str()));
    }
    delete library;
    return true;
   }
   fprintf(log, "WARNING: Library '%s' was not registered on this DefaultJvm instance!\n", path.c_str());
   return false;
  }
 };

 using DefaultJvm = JvmImpl<InvokeInterface, NativeInterface, JvmtiInterface, JniEnv, JvmtiEnv>;
}