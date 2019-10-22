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
  signal(SIGABRT, [](int signal) -> void {
//   if (signal == SIGABRT || signal == SIGSEGV || signal == SIGINT) {
//   }
   //TODO stop all JVM threads and force them to throw a fatal error
   for (auto& vm : Jvm::vms) {
    vm->fatalError("Received fatal signal!");
   }
   exit(signal);
  });
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
   while(vms[i]->uuid == std::string(str)) {
    str[rand() % 31] = randCharFunc();
    changed = true;
   }
   i = (changed ? 0 : i + 1);
  }
  str[32] = '\0';
  return str;
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

 bool Jvm::addInstance(JObject *inst) {
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

 //TODO search through all registered native classes for one containing a main method, and invoke it
 void Jvm::start() {
  if (running) {
   throw std::runtime_error("FATAL: Tried to start JVM instance twice!");
  }
  running = true;
  throw std::runtime_error("unimplemented");
//  running = false;
 }

 void Jvm::destroy() {
  running = false;
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

 void Jvm::fatalError(const char * message) {
  printf("FATAL: Fatal error thrown on Jvm instance '%s' with message: \n%s\n\n", uuid, message);
  printf("Backtrace: #STACK_FRAME STACK_POINTER[INSTRUCTION_POINTER]: (SYMBOL_NAME+OFFSET) SYMBOL_SOURCE\n");
  unw_cursor_t cursor, return_frame;
  unw_context_t uc;
  unw_word_t ip, sp, offset;
  //initialize frame to the current frame for local unwinding
  unw_getcontext(&uc);
  unw_init_local(&cursor, &uc);

  char sym[256];

  //unwind each frame up the stack
  bool startFound = false;
  int
   step,
   status = -1,
   frame = 0;
  while ((step = unw_step(&cursor)) > 0) {
   unw_get_reg(&cursor, UNW_REG_IP, &ip);
   unw_get_reg(&cursor, UNW_REG_SP, &sp);
   char * sym_name = sym;
   if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) != 0) {
    sym_name = const_cast<char *>("[stripped]");
   } else {
    char * demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);
    if (status == 0) {
     sym_name = demangled;
    }
   }
   if (strcmp(sym_name, "FakeJni::Jvm::start()") == 0) {
    if (!startFound) {
     return_frame = cursor;
     startFound = true;
    }
   }
   printf("#%d 0x%lx: (%s+0x%lx) [0x%lx]\n", frame, (intptr_t)sp, sym_name, (intptr_t)offset, (intptr_t)ip);
   //free demangled name
   if (status == 0) {
    free(sym_name);
   }
   frame += 1;
  }
  if (step != 0) {
   printf("FATAL: Encountered error unwinding stack!\n");
  }
  //return to enter
  //reuse `step` as a status indicator
  if (!startFound) {
   printf("FATAL: FakeJni::Jvm::start() entry point was not found on the stack!\n");
   exit(-1);
  }
  status = unw_resume(&return_frame);
  //if resume is successful this code will never be reached
  if (status != 0) {
   printf("FATAL: Resume failed with code: %d\n", status);
   exit(status);
  }
 }
}
