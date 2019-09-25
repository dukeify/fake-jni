#include "fake-jni/internal/util.h"
#include "fake-jni/jvm.h"
#include "fake-jni/array.h"
#include "fake-jni/string.h"

#include <algorithm>
#include <mutex>
#include <shared_mutex>

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

 AllocStack<Jvm *> Jvm::vms;

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
  vms.pushAlloc(this);
  registerDefaultClasses();
 }

 FILE * Jvm::getLog() const {
  return log;
 }

 void Jvm::registerDefaultClasses() {
  registerClass<JObject>();
  registerClass<JClass>();
  registerClass<JString>();
//  registerClass<JThrowable>();
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
  uint32_t i = 0;
  while (changed && i < vms.getSize()) {
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

 const AllocStack<const JClass *>& Jvm::getClasses() const {
  return classes;
 }

 AllocStack<JObject *>& Jvm::operator[](const JClass * clazz) {
  std::unique_lock lock(instances_mutex);
  return instances[clazz];
 }

 const AllocStack<JObject *>& Jvm::operator[](const JClass * clazz) const {
  std::shared_lock lock(instances_mutex);
  return (const_cast<Jvm&>(*this).instances)[clazz];
 }

 const decltype(Jvm::instances)& Jvm::getAllInstances() const {
  return instances;
 }

 bool Jvm::addInstance(JObject *inst) {
  auto& instances = (*this)[&inst->getClass()];
  if (!instances.contains(inst)) {
   instances.pushAlloc(inst);
   return true;
  }
  return false;
 }

 bool Jvm::removeInstance(FakeJni::JObject *inst) {
  return (*this)[&inst->getClass()].removeAlloc(inst);
 }

 bool Jvm::isRunning() const {
  return running;
 }

 Jvm::~Jvm() {
  vms.removeAlloc(this);
//  for (const auto &l : libraries) {
  const auto size = libraries.getSize();
  for (unsigned int i = 0; i < size; i++) {
   removeLibrary(libraries[i], "");
  }
  instances.clear();
  delete jvmtiEnv;
  delete jvmti;
  delete jniEnv;
  delete native;
  delete invoke;
  delete[] uuid;
 }

 bool Jvm::registerClass(const JClass * clazz) {
//  bool registered = std::find(classes.begin(), classes.end(), clazz) != classes.end();
  bool registered = classes.contains(clazz);
  if (!registered) {
//   for (const auto c : classes) {
   const auto size = classes.getSize();
   for (unsigned int i = 0; i < size; i++) {
    const auto& c = classes[i];
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
   classes.pushAlloc(clazz);
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
//  for (const auto clazz : classes) {
  const auto size = classes.getSize();
  for (unsigned int i = 0; i < size; i++) {
   const auto& clazz = classes[i];
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
  const auto size = libraries.getSize();
  bool libraryExists = false;
  for (unsigned int i = 0; i < size; i++) {
   if (libraries[i]->path == path) {
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
#ifdef FAKE_JNI_DEBUG
    fprintf(
     log,
     "DEBUG: JNI Linked for library: '%s'\nDEBUG: [%s]::JNI_OnLoad\n",
     path.c_str(),
     path.c_str()
    );
#endif
    if (!library->jniLoad()) {
     throw std::runtime_error("FATAL: Error initializing JNI library: '" + path + "'");
    }
   }
   //Only one startup agent function is called per library instance
   if (library->agentBound()) {
#ifdef FAKE_JNI_DEBUG
    fprintf(
     log,
     "DEBUG: Agent linked for library: '%s'\nDEBUG: [%s]::%s\n",
     path.c_str(),
     path.c_str(),
     (running ? "Agent_OnAttach" : "Agent_OnLoad")
    );
#endif
    const auto agentInitializer = (running ? &Library::agentAttach : &Library::agentLoad);
    if ((library->*agentInitializer)(const_cast<char *>(options.c_str()))) {
     throw std::runtime_error("FATAL: Error initializing agent library: '" + path + "'");
    }
   }
   libraries.pushAlloc([](void *lib) { delete (Library *)lib; }, library);
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
  const auto size = libraries.getSize();
  for (unsigned int i = 0; i < size; i++) {
   const auto library = libraries[i];
   if (library->path == path) {
    return removeLibrary(library, options);
   }
  }
  fprintf(log, "WARNING: Library '%s' was not registered on this Jvm instance!\n", path.c_str());
  return false;
 }

 bool Jvm::removeLibrary(const Library * library, const std::string & options) {
//   (void)std::remove(libs.begin(), libs.end(), library);
#ifdef FAKE_JNI_DEBUG
  fprintf(log, "DEBUG: Removing library: '%s'\n", library->path.c_str());
#endif
  if (library->jniBound()) {
   library->jniUnload();
  }
  if (library->agentBound()) {
   library->agentUnload(const_cast<char *>(options.c_str()));
  }
  libraries.removeAlloc(library);
//  delete library;
  return true;
 }

 const AllocStack<const Library *>& Jvm::getLibraries() const {
  return libraries;
 }

 //TODO search through all registered native classes for one containing a main method, and invoke it
 void Jvm::start() {
  if (running) {
   throw std::runtime_error("Tried to start JVM instance twice!");
  }
  running = true;
  throw std::runtime_error("unimplemented");
 }

 void Jvm::destroy() {
  running = false;
 }
}
