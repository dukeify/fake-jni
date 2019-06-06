#pragma once

#include "types.h"
#include "util.h"
#include "library.h"
#include "objects.h"
//Invoke implementations
#include "interface/invoke/invoke.h"
#include "interface/invoke/thread.h"
//Native implementations
#include "interface/native/native.h"
#include "interface/native/native_constructor.h"
#include "interface/native/native_inlined.h"
#include "interface/native/array.h"
#include "interface/native/buffer.h"
#include "interface/native/exception.h"
#include "interface/native/field.h"
#include "interface/native/method.h"
#include "interface/native/misc.h"
#include "interface/native/object.h"
#include "interface/native/ref.h"
#include "interface/native/reflect.h"
#include "interface/native/string.h"

#include "jni.h"

#include <vector>
#include <map>
#include <algorithm>
#include <memory>

namespace FakeJni {
 //TODO all JVM calls should be blocking until execution completes to prevent race conditions from emerging
 template<typename Invoke, typename Native, typename Env>
 class JvmImpl: public Jvm {
 private:
  FILE * const log;
  Invoke * const invoke;
  Native * const native;
  const Env env;

  //TODO on the first invocation of any JNI, JNIEnv or JVMTI functions, set this flag to true
  bool running;

  std::vector<Library *> libraries;

 public:
  explicit JvmImpl(): JvmImpl(stdout)
  {}

  explicit JvmImpl(FILE *log):
   JvmImpl(log, new Invoke(), new Native())
  {}

  explicit JvmImpl(FILE *log, Invoke * const invoke, Native * const native):
   Jvm(),
   log(log),
   invoke(invoke),
   native(native),
   env(Env(this))
  {
   functions = invoke;
   running = false;
  }

  //Upon DefaultJvm destruction, unload all libraries
  ~JvmImpl() {
   for (const auto &l : libraries) {
    removeLibrary(l->path, "");
   }
   libraries.clear();
  }

  FILE * getLog() override {
   return log;
  }

  Invoke * getInvokeInterface() override {
   return invoke;
  }

  Native * getNativeInterface() override {
   return native;
  }

  JniEnv * getEnv() override {
   return const_cast<JniEnv *>(&env);
  }

  //TODO search through all registered native classes for one containing a main method, and invoke it
  void start() override {
   throw std::runtime_error("");
  }

  void destroy() override {
   running = false;
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

 using DefaultJvm = JvmImpl<InvokeInterface, NativeInterface, JniEnv>;
}