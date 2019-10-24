#include "fake-jni/internal/util.h"
#include "fake-jni/jvm.h"
#include "fake-jni/array.h"
#include "fake-jni/string.h"
#include "fake-jni/throwable.h"

#include <algorithm>
#include <mutex>
#include <shared_mutex>
#include <csignal>
#include <cxxabi.h>

#define UNW_LOCAL_ONLY
#include <libunwind.h>

#define DEFAULT_MANGLED_SYMBOL_NAME_CACHE 4096

#define _UNW_SUCCEED_OR_THROW(name, ...)\
if ((unw_status = name(__VA_ARGS__))) {\
 throw UnwindException("FATAL: " + std::string(#name) + "() failed with error code: " + std::to_string(unw_status));\
}

#define _UNW_SUCCEED_OR_EXIT(name, ...)\
if ((unw_status = name(__VA_ARGS__))) {\
 fprintf(log, "FATAL: %s() failed with error code: %d\n", #name, unw_status);\
 exit(-1);\
}

//Non-template members of Jvm
namespace FakeJni {
 //Class descriptors for all primitives
 BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR(JVoid, voidDescriptor) END_NATIVE_DESCRIPTOR
 BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR(JBoolean, booleanDescriptor) END_NATIVE_DESCRIPTOR
 BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR(JByte, byteDescriptor) END_NATIVE_DESCRIPTOR
 BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR(JChar, shortDescriptor) END_NATIVE_DESCRIPTOR
 BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR(JShort, intDescriptor) END_NATIVE_DESCRIPTOR
 BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR(JInt, floatDescriptor) END_NATIVE_DESCRIPTOR
 BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR(JFloat, longDescriptor) END_NATIVE_DESCRIPTOR
 BEGIN_NATIVE_PRIMITIVE_DESCRIPTOR(JLong, doubleDescriptor) END_NATIVE_DESCRIPTOR

 PointerList<Jvm *> Jvm::vms;
 thread_local const Jvm *Jvm::currentVm;

 Jvm::Jvm(FILE *log) :
  JavaVM(),
  uuid(generateJvmUuid()),
  log(log),
  invoke(new InvokeInterface(*this)),
  native(new NativeInterface(*this)),
  jvmti(new JvmtiInterface(*this)),
  jniEnv(new JniEnv(*this)),
  jvmtiEnv(new JvmtiEnv(*this)),
  libraries{true},
  classes{true}
 {
  functions = invoke;
  vms.insert(this);
  registerDefaultClasses();
 }

 FILE * Jvm::getLog() const {
  return log;
 }

 void Jvm::registerDefaultClasses() {
  registerClass<JObject>();
  registerClass<JClass>();
  registerClass<JString>();
  registerClass<JThrowable>();
  registerClass<JArray<JThrowable *>>();
//  registerClass<JWeak>();
  registerClass<JBooleanArray>();
  registerClass<JByteArray>();
  registerClass<JCharArray>();
  registerClass<JShortArray>();
  registerClass<JIntArray>();
  registerClass<JFloatArray>();
  registerClass<JLongArray>();
  registerClass<JDoubleArray>();
  registerClass<JObjectArray>();
  registerClass(&voidDescriptor);
  registerClass(&booleanDescriptor);
  registerClass(&byteDescriptor);
  registerClass(&shortDescriptor);
  registerClass(&intDescriptor);
  registerClass(&floatDescriptor);
  registerClass(&longDescriptor);
  registerClass(&doubleDescriptor);
 }

 void Jvm::registerDefaultSignalHandler() {
  static struct sigaction new_sa, old_sa;
  new_sa.sa_handler = [](int sig) -> void {
   //vm is a thread_local variable that is only set when Jvm::start() is invoked, and is unset when it returns
   //if vm is not set, then no vm is currently running and the previous signal handler should be used
   const auto vm = Jvm::getCurrentVm();
   if (vm) {
    vm->fatalError("Received fatal signal!");
   } else {
    if (old_sa.sa_handler) {
     (*old_sa.sa_handler)(sig);
    }
   }
  };
  sigemptyset(&new_sa.sa_mask);
  if (sigaction(SIGABRT, &new_sa, &old_sa) == -1) {
   throw std::runtime_error("FATAL: Could not set the default Jvm signal handler!");
  }
 }

 const char * Jvm::generateJvmUuid() noexcept {
  static auto randCharFunc = []() -> char {
   return (char)((rand() % 2 ? 65 : 97) + (rand() % 25));
  };
  auto str = new char[33];
  for (uint32_t i = 0; i < 32; i++) {
   str[i] = randCharFunc();
  }
  //Ensure that the uuid is unique
  bool changed = true;
  auto size = vms.size();
  decltype(size) i = 0;
  while (changed && i < size) {
   changed = false;
   while (vms[i]->uuid == std::string(str)) {
    str[rand() % 31] = randCharFunc();
    changed = true;
   }
   i = (changed ? 0 : i + 1);
  }
  str[32] = '\0';
  return str;
 }

 const Jvm * Jvm::getCurrentVm() noexcept {
  return currentVm;
 }

 inline void Jvm::setInvokeInterface(InvokeInterface * const ii) {
  delete invoke;
  invoke = ii;
  functions = ii;
 }

 inline InvokeInterface& Jvm::getInvokeInterface() const {
  return *invoke;
 }

 inline void Jvm::setNativeInterface(NativeInterface * const ni) {
  delete native;
  native = ni;
  jniEnv->functions = ni;
 }

 inline NativeInterface& Jvm::getNativeInterface() const {
  return *native;
 }

 inline void Jvm::setJvmtiInterface(JvmtiInterface * const ji) {
  delete jvmti;
  jvmti = ji;
  jvmtiEnv->functions = ji;
 }

 inline JvmtiInterface& Jvm::getJvmtiInterface() const {
  return *jvmti;
 }

 inline void Jvm::setJniEnv(JniEnv * const env) {
  auto interface = (NativeInterface *)jniEnv->functions;
  delete jniEnv;
  jniEnv = env;
  jniEnv->setNativeInterface(*interface);
 }

 inline JniEnv& Jvm::getJniEnv() const {
  return *jniEnv;
 }

 inline void Jvm::setJvmtiEnv(JvmtiEnv * const env) {
  auto interface = (JvmtiInterface *)env->functions;
  delete jvmtiEnv;
  jvmtiEnv = env;
  jvmtiEnv->setJvmtiInterface(*interface);
 }

 inline JvmtiEnv& Jvm::getJvmtiEnv() const {
  return *jvmtiEnv;
 }

 const PointerList<const JClass *>& Jvm::getClasses() const {
  return classes;
 }

 PointerList<JObject *>& Jvm::operator[](const JClass * clazz) {
  std::unique_lock lock(instances_mutex);
  return instances[clazz];
 }

 const PointerList<JObject *>& Jvm::operator[](const JClass * clazz) const {
  std::shared_lock lock(instances_mutex);
  return (const_cast<Jvm&>(*this).instances)[clazz];
 }

 const decltype(Jvm::instances)& Jvm::getAllInstances() const {
  return instances;
 }

 bool Jvm::addInstance(JObject * inst) {
  auto& instances = (*this)[&inst->getClass()];
  if (!instances.contains(inst)) {
   instances.insert(inst, nullptr);
   return true;
  }
  return false;
 }

 bool Jvm::removeInstance(JObject * inst) {
  auto & instances = (*this)[&inst->getClass()];
  return instances.end() != instances.erase(inst);
 }

 bool Jvm::isRunning() const {
  return running;
 }

 Jvm::~Jvm() {
  vms.erase(this);
  for (auto library : libraries) {
   removeLibrary(library, "");
  }
  instances.clear();
  delete jvmtiEnv;
  delete jvmti;
  delete jniEnv;
  delete native;
  delete invoke;
  delete[] uuid;
 }

 bool Jvm::registerClass(const JClass * clazz, bool deallocate) {
  bool registered = classes.contains(clazz);
  if (!registered) {
   for (auto c : classes) {
    if (strcmp(c->getName(), clazz->getName()) == 0) {
     registered |= true;
     break;
    }
   }
  }
  if (registered) {
#ifdef FAKE_JNI_DEBUG
   fprintf(
    log,
    "WARNING: Class '%s' is already registered on the JVM instance '%s'!\n",
    clazz->getName(),
    uuid
   );
#endif
   return false;
  } else {
   std::unique_lock lock(instances_mutex);
   instances[clazz].setDeallocate(true);
   if (deallocate) {
    classes.insert(clazz);
   } else {
    classes.insert(clazz, nullptr);
   }
  }
  return true;
 }

 bool Jvm::unregisterClass(const JClass * clazz) {
//  const auto end = classes.end();
//  const auto found = end != classes.erase(std::remove(classes.begin(), end, clazz), end);
  const auto found = classes.contains(clazz);
#ifdef FAKE_JNI_DEBUG
  if (!found) {
   fprintf(
    log,
    "WARNING: Class '%s' is not registered on the JVM instance '%s'!\n",
    clazz->getName(),
    uuid
   );
  }
#endif
  return found;
 }

 const JClass * Jvm::findClass(const char * name) const {
  for (auto clazz : classes) {
   if (strcmp(name, clazz->getName()) == 0) {
    return clazz;
   }
  }
  return nullptr;
 }

 void Jvm::attachLibrary(
  const std::string & rpath,
  const std::string & options,
  LibraryOptions loptions
 ) {
  std::string path = rpath.empty() ? "(embedded)" : rpath;
  bool libraryExists = false;
  for (auto lib : libraries) {
   if (lib->path == path) {
    libraryExists = true;
    break;
   }
  }
  if (!libraryExists) {
   auto library = new Library(const_cast<Jvm&>(*this), path, loptions);
#ifdef FAKE_JNI_DEBUG
   fprintf(log, "DEBUG: Created library: '%s'\n", path.c_str());
#endif
   if (library->jniBound()) {
    if (!library->jniLoad()) {
     throw std::runtime_error("FATAL: Error initializing JNI library: '" + path + "'");
    }
   }
   //Only one startup agent function is called per library instance
   if (library->agentBound()) {
    const auto agentInitializer = (running ? &Library::agentAttach : &Library::agentLoad);
    if ((library->*agentInitializer)(const_cast<char *>(options.c_str()))) {
     throw std::runtime_error("FATAL: Error initializing agent library: '" + path + "'");
    }
   }
   libraries.insert(library);
#ifdef FAKE_JNI_DEBUG
   fprintf(log, "DEBUG: Registered library: '%s'\n", path.c_str());
#endif
  }
#ifdef FAKE_JNI_DEBUG
  else {
   fprintf(log, "WARNING: Library '%s' is already registered on this DefaultJvm instance!\n", path.c_str());
  }
#endif
 }

 //Removes a library from the DefaultJvm instance
 //Implicitly unloads the agent within the library, if there was one
 //Implicitly unloads the jni component within the library, if there was one
 bool Jvm::removeLibrary(const std::string & path, const std::string & options) {
  for (auto library : libraries) {
   if (library->path == path) {
    return removeLibrary(library, options);
   }
  }
  fprintf(log, "WARNING: Library '%s' was not registered on this Jvm instance!\n", path.c_str());
  return false;
 }

 bool Jvm::removeLibrary(const Library * library, const std::string & options) {
#ifdef FAKE_JNI_DEBUG
  fprintf(log, "DEBUG: Removing library: '%s'\n", library->path.c_str());
#endif
  if (library->jniBound()) {
   library->jniUnload();
  }
  if (library->agentBound()) {
   library->agentUnload(const_cast<char *>(options.c_str()));
  }
  libraries.erase(library);
//  delete library;
  return true;
 }

 const PointerList<const Library *>& Jvm::getLibraries() const {
  return libraries;
 }

 void Jvm::start() {
  if (running) {
   throw std::runtime_error("FATAL: Tried to start JVM instance twice!");
  }
  currentVm = this;
  running = true;
  //TODO REMOVE
  throw std::runtime_error("unimplemented");
  try {
   const JClass * encapsulatingClass = nullptr;
   const JMethodID * main = nullptr;
   for (auto& clazz : classes) {
    for (auto& mid : clazz->getMethods()) {
     if (strcmp(mid->getName(), "main") == 0) {
      if (strcmp(mid->getSignature(), "([java/lang/String;)V") == 0) {
       encapsulatingClass = clazz;
       main = mid;
      }
     }
     break;
    }
   }
   if (!main) {
    throw std::runtime_error("FATAL: No classes define the default Java entry point: 'main([Ljava/lang/String;)V'!");
   }
   main->invoke<void>(this, encapsulatingClass);
  } catch(const std::exception &ex) {
   //TODO
   fprintf(log, "FATAL: VM encountered an uncaught exception with message:\n%s\n", ex.what());
//   throw;
  } catch(...) {
   //TODO
   fprintf(log, "FATAL: VM encountered an unknown fatal error!\n");
//   throw;
  }
  running = false;
  currentVm = nullptr;
 }

 //TODO
 JInt Jvm::destroy() {
//  running = false;
  return JNI_OK;
 }

 void Jvm::throwException(jthrowable throwable) {
  clearException();
  exception = throwable;
 }

 jthrowable Jvm::getException() const {
  return exception;
 }

 void Jvm::clearException() {
  exception = nullptr;
 }

 void Jvm::fatalError(const char * message) const {
  printf("FATAL: Fatal error thrown on Jvm instance '%s' with message: \n%s\n\n", uuid, message);
  try {
   printBacktrace();
  } catch (UnwindException &e) {
   fprintf(log, "FATAL: Encountered exception unwinding stack:\n%s\n", e.what());
   exit(-1);
  } catch (...) {
   fprintf(log, "FATAL: Encountered unexpected exception unwinding stack!\n");
   exit(-1);
  }
  unw_cursor_t cursor;
  unw_context_t uc;
  unw_word_t off;
  int
   unw_status,
   demangle_status = -1,
   symbol_size = DEFAULT_MANGLED_SYMBOL_NAME_CACHE - 1024;
  bool startFound = false;

  //TODO put the Jvm into an errored state so that the user can handle the error
  //find FakeJni::Jvm::start() and continue execution at that frame
  //initialize frame to the current frame for local unwinding
  _UNW_SUCCEED_OR_EXIT(unw_getcontext, &uc)
  _UNW_SUCCEED_OR_EXIT(unw_init_local, &cursor, &uc)
  //unwind each frame up the stack
  while ((unw_status = unw_step(&cursor)) > 0) {
   //resolve mangled symbol name
   //will continue until the buffer is large enough to contain it
   do {
    char mangled[(symbol_size += 1024)];
    unw_status = unw_get_proc_name(&cursor, mangled, sizeof(mangled), &off);
   } while (unw_status == UNW_ENOMEM);
   if (unw_status) {
    fprintf(log, "FATAL: unw_get_proc_name() failed with error code: %d\n", unw_status);
    exit(-1);
   }
   char mangled[symbol_size];
   _UNW_SUCCEED_OR_EXIT(unw_get_proc_name, &cursor, mangled, sizeof(mangled), &off)
   char *sym = abi::__cxa_demangle(mangled, nullptr, nullptr, &demangle_status);
   if (demangle_status == -2) {
    sym = mangled;
   } else if (demangle_status) {
#ifdef FAKE_JNI_DEBUG
    fprintf(log, "WARNING: Could not demangle symbol: %s\n", mangled);
#endif
    continue;
   }
   if (strcmp(sym, "FakeJni::Jvm::start()") == 0) {
    startFound = true;
   }
   if (!demangle_status) {
    free(sym);
   }
   if (startFound) {
    break;
   }
  }
  if (unw_status < 0) {
   fprintf(log, "FATAL: unw_step() failed with error code: %d\n", unw_status);
   exit(-1);
  }
  if (!startFound) {
   fprintf(log, "FATAL: FakeJni::Jvm::start() entry point was not found on the stack!\n");
   exit(-1);
  }
  //if resume is successful this code will never be reached
  unw_status = unw_resume(&cursor);
  fprintf(log, "FATAL: unw_resume() failed with error code: %d\n", unw_status);
  exit(-1);
 }

 void Jvm::printBacktrace() const {
  printf("Backtrace: #STACK_FRAME STACK_POINTER: (SYMBOL_NAME+OFFSET) [INSTRUCTION_POINTER] in SYMBOL_SOURCE\n");
  unw_cursor_t cursor;
  unw_context_t uc;
  unw_word_t ip, sp, off;
  unw_proc_info_t pip;
  int
   unw_status,
   demangle_status = -1,
   frame_number = 0,
   symbol_size = DEFAULT_MANGLED_SYMBOL_NAME_CACHE - 1024;
  Dl_info dlinfo;
  const char *sym, *obj_file;

  _UNW_SUCCEED_OR_THROW(unw_getcontext, &uc)
  _UNW_SUCCEED_OR_THROW(unw_init_local, &cursor, &uc)
  while ((unw_status = unw_step(&cursor)) > 0) {
   pip.unwind_info = nullptr;
   //get process info
   _UNW_SUCCEED_OR_THROW(unw_get_proc_info, &cursor, &pip)
   //TODO maybe we should log this?
//  printf();
   //get instruction and stack pointers
   _UNW_SUCCEED_OR_THROW(unw_get_reg, &cursor, UNW_REG_IP, &ip)
   _UNW_SUCCEED_OR_THROW(unw_get_reg, &cursor, UNW_REG_SP, &sp)
   //resolve mangled symbol name
   //will continue until the buffer is large enough to contain it
   do {
    char mangled[(symbol_size += 1024)];
    unw_status = unw_get_proc_name(&cursor, mangled, sizeof(mangled), &off);
   } while (unw_status == UNW_ENOMEM);
   if (unw_status) {
    throw UnwindException("FATAL: unw_get_proc_name() failed with error code: " + std::to_string(unw_status));
   }
   //get symbol name and demangle if possible
   char mangled[symbol_size];
   sym = mangled;
   if (unw_get_proc_name(&cursor, mangled, sizeof(mangled), &off)) {
    sym = "[stripped]";
   } else {
    char *demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &demangle_status);
    if (!demangle_status) {
     sym = demangled;
    }
   }
   //resolve object file name
   if (dladdr((void *)(pip.start_ip + off), &dlinfo) && dlinfo.dli_fname && *dlinfo.dli_fname) {
    obj_file = dlinfo.dli_fname;
   } else {
    obj_file = "[stripped]";
   }
   //#frame_num stack_ptr, (symbol_name+offset) [instruction_ptr] in object_file
   fprintf(
    log,
    "#%2d 0x%lx: (%s+0x%lx) [0x%lx] in %s\n",
    frame_number,
    (intptr_t)sp,
    sym,
    (intptr_t)off,
    (intptr_t)ip,
    obj_file
   );
   //if the name was demangled, free the allocated string
   if (!demangle_status) {
    free((void *)sym);
   }
   frame_number += 1;
  }
  if (unw_status != 0) {
   throw UnwindException("FATAL: unw_step() failed with error code: " + std::to_string(unw_status));
  }
 }
}