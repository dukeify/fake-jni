#include "jni.h"

#include "fake-jni/jvm.h"

#define UNBOUND_AGENT \
throw std::runtime_error("FATAL: Cannot invoke unbound agent hooks for library: '" + path + "'!");

#define UNBOUND_JNI \
throw std::runtime_error("FATAL: Cannot invoke unbound JNI hooks for library: '" + path + "'!");

namespace FakeJni {
 Library::Library(Jvm * const vm, const std::string path, const LibraryOptions options) :
  vm(vm),
  path(path),
  options(options)
 {
  const char * const name = (path == "(embedded)" ? nullptr : path.c_str());
  handle = options.dlopen_p(name, RTLD_LAZY);
  if (!handle) {
   throw std::runtime_error("FATAL: Failed to open library: '" + path + "'!");
  }
  //clear any previous errors
  dlerror();
  char * error;
  if (((void *&)JNI_OnLoad_ = lsym("JNI_OnLoad_L"))) {
   error = dlerror();
   //Load static JNI linkage (OnUnload is optional)
   if (error == nullptr) {
#ifdef FAKE_JNI_DEBUG
    fprintf(vm->getLog(), "DEBUG: '%s' uses static JNI linkage\n", path.c_str());
#endif
    (void *&)JNI_OnUnload_ = lsym("JNI_OnUnload_L");
   } else {
    fprintf(vm->getLog(), "WARNING: ");
   }
  } else if (((void *&)JNI_OnLoad_ = lsym("JNI_OnLoad"))) {
   error = dlerror();
   if (error == nullptr) {
#ifdef FAKE_JNI_DEBUG
    fprintf(vm->getLog(), "DEBUG: '%s' uses dynamic JNI linkage\n", path.c_str());
#endif
    //Load dynamic JNI linkage (unload not required)
    (void *&)JNI_OnUnload_ = lsym("JNI_OnUnload");
   }
  }
#ifdef FAKE_JNI_DEBUG
  else {
   fprintf(vm->getLog(), "DEBUG: '%s' contains no JNI linkage\n", path.c_str());
  }
#endif

  //clear any previous errors
  dlerror();
  if (((void *&)Agent_OnLoad_ = lsym("Agent_OnLoad_L"))) {
   error = dlerror();
   if (error == nullptr) {
#ifdef FAKE_JNI_DEBUG
    fprintf(vm->getLog(), "DEBUG: '%s' uses static Agent linkage\n", path.c_str());
#endif
    //Load static Agent linkage (OnAttach and OnUnload are optional)
    (void *&)Agent_OnAttach_ = lsym("Agent_OnAttach_L");
    (void *&)Agent_OnUnload_ = lsym("Agent_OnUnload_L");
   }
  } else if (((void *&)Agent_OnLoad_ = lsym("Agent_OnLoad"))) {
   error = dlerror();
   if (error == nullptr) {
#ifdef FAKE_JNI_DEBUG
    fprintf(vm->getLog(), "DEBUG: '%s' uses dynamic Agent linkage\n", path.c_str());
#endif
    //Load dynamic Agent linkage (OnAttach and OnUnload are optional)
    (void *&)Agent_OnAttach_ = lsym("Agent_OnAttach");
    (void *&)Agent_OnUnload_ = lsym("Agent_OnUnload");
   }
  }
#ifdef FAKE_JNI_DEBUG
  else {
   fprintf(vm->getLog(), "DEBUG: '%s' contains no Agent linkage\n", path.c_str());
  }
#endif
  if (!(agentBound() || jniBound())) {
   fprintf(vm->getLog(), "WARNING: Neither JNI nor Agent hooks were found for library: '%s'!\n", path.c_str());
  }
 }

 Library::~Library() {
#ifdef FAKE_JNI_DEBUG
  fprintf(vm->getLog(), "DEBUG: Closing handle for library '%s'\n", path.c_str());
#endif
  const int status = options.dlclose_p(handle);
  if (status) {
#ifdef FAKE_JNI_DEBUG
   fprintf(
    vm->getLog(),
    "WARNING: Failed to close dl handle for '%s', with error: %s",
    path.c_str(),
    options.dlerror_p()
   );
#endif
  }
 }

 void * Library::lsym(const char *symbol) {
  return options.dlsym_p(handle, symbol);
 }

 bool Library::agentBound() {
  return Agent_OnAttach_ || Agent_OnLoad_ || Agent_OnUnload_;
 }

 bool Library::jniBound() {
  return JNI_OnLoad_ || JNI_OnUnload_;
 }

 JInt Library::agentAttach(char * const options) {
  if (agentBound()) {
   return Agent_OnAttach_ ? Agent_OnAttach_(vm, options, nullptr) : JNI_OK;
  }
  UNBOUND_AGENT
 }

 JInt Library::agentLoad(char * const options) {
  if (agentBound()) {
   return Agent_OnLoad_ ? Agent_OnLoad_(vm, options, nullptr) : JNI_OK;
  }
  UNBOUND_AGENT
 }

 void Library::agentUnload(char * const options) {
  if (agentBound()) {
   if (Agent_OnUnload_) {
    Agent_OnUnload_(vm, options, nullptr);
   }
  }
  UNBOUND_AGENT
 }

 JInt Library::jniLoad() {
  if (jniBound()) {
   return JNI_OnLoad_(vm, nullptr);
  }
  UNBOUND_JNI
 }

 void Library::jniUnload() {
  if (jniBound()) {
   if (JNI_OnUnload_) {
    JNI_OnUnload_(vm, nullptr);
   }
  }
  UNBOUND_JNI
 }
}