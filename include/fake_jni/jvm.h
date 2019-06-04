#pragma once

#include "fake_jni/library.h"
#include "fake_jni/types.h"
#include "fake_jni/jvm_invoke.h"
#include "fake_jni/jvm_native.h"

#include "jni.h"

#include <vector>
#include <map>
#include <algorithm>
#include <memory>

namespace FakeJVM {
 //TODO all JVM calls should be blocking until execution completes to prevent race conditions from emerging
 class DefaultJvm: public Jvm {
 private:
  FILE * const log;
  const InvokeInterface invoke;
  const NativeInterface native;
  const JNIEnv env;

  //TODO on the first invocation of any JNI, JNIEnv or JVMTI functions, set this flag to true
  bool running;

  std::vector<Library *> libraries;

 public:
  explicit DefaultJvm(): DefaultJvm(stdout)
  {}

  explicit DefaultJvm(FILE *log):
   DefaultJvm(log, InvokeInterface(), NativeInterface())
  {}

  explicit DefaultJvm(FILE *log, const InvokeInterface &invoke, const NativeInterface &native):
   Jvm(),
   log(log),
   invoke(invoke),
   native(native),
   env { &(this->native) }
  {
   functions = &(this->invoke);
   running = false;
  }

  //Upon DefaultJvm destruction, unload all libraries
  ~DefaultJvm() {
   for (const auto &l : libraries) {
    removeLibrary(l->path, "");
   }
   libraries.clear();
  }

  FILE * getLog() override {
   return log;
  }

  InvokeInterface getInvokeInterface() override {
   return invoke;
  }

  NativeInterface getNativeInterface() override {
   return native;
  }

  JNIEnv * getEnv() override {
   return const_cast<JNIEnv *>(&env);
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
}