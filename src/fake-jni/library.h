#pragma once

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
 using dlclose_t = int (* const)(void *handle);
 using dlerror_t = char* (* const)();

 class LibraryOptions final {
 public:
  dlopen_t dlopen_p;
  dlsym_t dlsym_p;
  dlclose_t dlclose_p;
  dlerror_t dlerror_p;

  LibraryOptions(
   dlopen_t dlopen_p = &dlopen,
   dlsym_t dlsym_p = &dlsym,
   dlclose_t dlclose_p = &dlclose,
   dlerror_t dlerror_p = &dlerror
  ) :
   dlopen_p(dlopen_p),
   dlsym_p(dlsym_p),
   dlclose_p(dlclose_p),
   dlerror_p(dlerror_p)
  {}
 };

 //Represents both JNI Agents and JNI Libraries (can only be bound to a single DefaultJvm instance at a time)
 //Dynamic Agents use Agent_OnAttach, Agent_OnLoad and Agent_OnUnload
 //Static Agents use Agent_OnAttach_L, Agent_OnLoad_L, Agent_OnUnload_L
 //Dynamic Libraries use JNI_OnLoad and JNI_OnUnload
 //Static Libraries use JNI_OnLoad_L and JNI_OnUnload_L
 class Library final {
 private:
  void *handle;
  jint (*JNI_OnLoad_)(JavaVM *vm, void *reserved);
  void (*JNI_OnUnload_)(JavaVM *vm, void *reserved);
  jint (*Agent_OnAttach_)(JavaVM *vm, char *options, void *reserved);
  jint (*Agent_OnLoad_)(JavaVM *vm, char *options, void *reserved);
  void (*Agent_OnUnload_)(JavaVM *vm, char *options, void *reserved);

  bool determineIsStatic(const std::string &path) {
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

 public:
  Jvm * const vm;
  const std::string path;
  const LibraryOptions options;
  const bool isStatic;

  Library(Jvm * const vm, const std::string path, const LibraryOptions options):
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

  ~Library() {
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

  void * lsym(const char *symbol) {
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

  bool agentBound() {
   return Agent_OnAttach_ || Agent_OnLoad_ || Agent_OnUnload_;
  }

  bool jniBound() {
   return JNI_OnLoad_ || JNI_OnUnload_;
  }

  jint agentAttach(char *options) {
   return Agent_OnAttach_ ? Agent_OnAttach_(vm, options, nullptr) : JNI_OK;
  }

  jint agentLoad(char *options) {
   return Agent_OnLoad_ ? Agent_OnLoad_(vm, options, nullptr) : JNI_OK;
  }

  void agentUnload(char *options) {
   if (Agent_OnUnload_) {
    Agent_OnUnload_(vm, options, nullptr);
   }
  }

  jint jniLoad() {
   return JNI_OnLoad_(vm, nullptr);
  }

  void jniUnload() {
   if (JNI_OnUnload_) {
    JNI_OnUnload_(vm, nullptr);
   }
  }
 };
}
