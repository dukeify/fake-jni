#include "jni.h"

#include "fake-jni/jvm.h"

#define UNBOUND_AGENT \
throw std::runtime_error("FATAL: Cannot invoke unbound agent hooks for library: '" + path + "'!");

#define UNBOUND_JNI \
throw std::runtime_error("FATAL: Cannot invoke unbound JNI hooks for library: '" + path + "'!");

namespace FakeJni {
// bool Library::determineIsStatic(const std::string &path) {
//  if (path == "(embedded)") {
//#ifdef FAKE_JNI_DEBUG
//   fprintf(
//    vm->getLog(),
//    "WARNING: Could not determine the JNI/Agent linkage type for embedded library, assuming dynamic!\n"
//   );
//#endif
//   return false;
//  } else {
//   const unsigned long pSize = path.size();
//   return path.substr(pSize - STATIC_LIB_SUFFIX.size(), pSize - 1) == STATIC_LIB_SUFFIX;
//  }
// }

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
  const bool
//   jniIsStatic = false,
   agentIsStatic = !lsym("Agent_OnAttach_L");
//#ifdef FAKE_JNI_DEBUG
//  fprintf(
//   vm->getLog(),
//   "DEBUG: '%s' uses %s JNI linkage!\n",
//   path.c_str(),
//   jniIsStatic ? "static" : "dynamic"
//  );
//#endif
  if (lsym("JNI_OnLoad_L") && dlerror() == nullptr) {
   //Load static JNI linkage
   (void *&)JNI_OnLoad_ = lsym("JNI_OnLoad_L");
   (void *&)JNI_OnUnload_ = lsym("JNI_OnUnload_L");
  } else {
   //Load dynamic JNI linkage
   (void *&)JNI_OnLoad_ = lsym("JNI_OnLoad");
   (void *&)JNI_OnUnload_ = lsym("JNI_OnUnload");
  }
#ifdef FAKE_JNI_DEBUG
  fprintf(
   vm->getLog(),
   "DEBUG: '%s' uses %s Agent linkage!\n",
   path.c_str(),
   agentIsStatic ? "static" : "dynamic"
  );
#endif
  if (agentIsStatic) {
   (void *&)Agent_OnAttach_ = lsym("Agent_OnAttach_L");
   (void *&)Agent_OnLoad_ = lsym("Agent_OnLoad_L");
   (void *&)Agent_OnUnload_ = lsym("Agent_OnUnload_L");
  } else {
   (void *&)Agent_OnAttach_ = lsym("Agent_OnAttach");
   (void *&)Agent_OnLoad_ = lsym("Agent_OnLoad");
   (void *&)Agent_OnUnload_ = lsym("Agent_OnUnload");
  }
  //Note: Agent linkage is optional, no checking required
  //Note: Incomplete JNI linkage is not fatal on the DefaultJvm, the library is just ignored
  if (!(agentBound() || jniBound())) {
   fprintf(vm->getLog(), "WARNING: Neither JNI nor Agent hooks were found for library: '%s'!\n", path.c_str());
  }
 }

 Library::~Library() {
#ifdef FAKE_JNI_DEBUG
  fprintf(vm->getLog(), "DEBUG: Closing handle for library '%s'", path.c_str());
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
//  void * const ret = options.dlsym_p(handle, symbol);
//  const char * const err = options.dlerror_p();
//  if (err) {
//#ifdef FAKE_JNI_DEBUG
//   fprintf(
//    vm->getLog(),
//    "WARNING: Failed to load symbol '%s' in library '%s', with error: \n\t%s\n\n",
//    symbol,
//    path.c_str(),
//    err
//   );
//#endif
//  }
//  return ret;
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