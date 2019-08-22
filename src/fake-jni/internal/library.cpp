#include "jni.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
 bool Library::determineIsStatic(const std::string &path) {
  if (path == "(embedded)") {
   //TODO once the JVM compliant linkage detection is in place, this system will be removed
#ifdef FAKE_JNI_DEBUG
   fprintf(
    vm->getLog(),
    "WARNING: Could not determine the JNI/Agent linkage type for embedded library, assuming dynamic!\n"
   );
#endif
   return false;
  } else {
   const unsigned long pSize = path.size();
   return path.substr(pSize - STATIC_LIB_SUFFIX.size(), pSize - 1) == STATIC_LIB_SUFFIX;
  }
 }

 Library::Library(Jvm * const vm, const std::string path, const LibraryOptions options) :
  vm(vm),
  path(path),
  options(options),
  isStatic(determineIsStatic(path))
 {
  const char *name = (path == "(embedded)" ? nullptr : path.c_str());
  handle = options.dlopen_p(name, RTLD_LAZY);
  if (!handle) {
   throw std::runtime_error("FATAL: Failed to open library: '" + path + "'!");
  }
  //TODO JNI spec permits libraries to define both the static and dynamic exported functions,
  // with selective delegation in such a case (supersedes actual library linkage)
  if (isStatic) {
#ifdef FAKE_JNI_DEBUG
   fprintf(vm->getLog(), "DEBUG: '%s' uses static JNI linkage!\n", path.c_str());
#endif
   //Load static JNI linkage
   (void *&)JNI_OnLoad_ = lsym("JNI_OnLoad_L");
   (void *&)JNI_OnUnload_ = lsym("JNI_OnUnload_L");
   (void *&)Agent_OnAttach_ = lsym("Agent_OnAttach_L");
   (void *&)Agent_OnLoad_ = lsym("Agent_OnAttach_L");
   (void *&)Agent_OnUnload_ = lsym("Agent_OnUnload_L");
  } else {
#ifdef FAKE_JNI_DEBUG
   fprintf(vm->getLog(), "DEBUG: '%s' uses dynamic JNI linkage!\n", path.c_str());
#endif
   //Load dynamic JNI linkage
   (void *&)JNI_OnLoad_ = lsym("JNI_OnLoad");
   (void *&)JNI_OnUnload_ = lsym("JNI_OnUnload");
   (void *&)Agent_OnAttach_ = lsym("Agent_OnAttach");
   (void *&)Agent_OnLoad_ = lsym("Agent_OnLoad");
   (void *&)Agent_OnUnload_ = lsym("Agent_OnUnload");
  }
  //Note: Agent linkage is optional, no checking required
  //Note: Incomplete JNI linkage is not fatal on the DefaultJvm, the library is just ignored
  //TODO should this be fatal?
  if (!(agentBound() || jniBound())) {
   throw std::runtime_error("FATAL: Neither JNI nor Agent hooks were found for library: '" + path + "'!");
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
  void * const ret = options.dlsym_p(handle, symbol);
  const char * const err = options.dlerror_p();
  if (err) {
#ifdef FAKE_JNI_DEBUG
   fprintf(
    vm->getLog(),
    "WARNING: Failed to load symbol '%s' in library '%s', with error: \n\t%s\n\n",
    symbol,
    path.c_str(),
    err
   );
#endif
  }
  return ret;
 }

 bool Library::agentBound() {
  return Agent_OnAttach_ || Agent_OnLoad_ || Agent_OnUnload_;
 }

 bool Library::jniBound() {
  return JNI_OnLoad_ || JNI_OnUnload_;
 }

 JInt Library::agentAttach(char *options) {
  return Agent_OnAttach_ ? Agent_OnAttach_(vm, options, nullptr) : JNI_OK;
 }

 JInt Library::agentLoad(char *options) {
  return Agent_OnLoad_ ? Agent_OnLoad_(vm, options, nullptr) : JNI_OK;
 }

 void Library::agentUnload(char *options) {
  if (Agent_OnUnload_) {
   Agent_OnUnload_(vm, options, nullptr);
  }
 }

 JInt Library::jniLoad() {
  return JNI_OnLoad_(vm, nullptr);
 }

 void Library::jniUnload() {
  if (JNI_OnUnload_) {
   JNI_OnUnload_(vm, nullptr);
  }
 }
}