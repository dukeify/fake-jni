#include "jni.h"

#include "fake-jni/internal/util.h"
#include "fake-jni/jvm.h"

#include <algorithm>

//Non-template members of Jvm
namespace FakeJni {
 AllocStack<Jvm *> Jvm::vms;

 void Jvm::generateJvmUuid(char * const uuid) {
  static auto randCharFunc = []() -> char {
   return (char)((rand() % 2 ? 65 : 97) + (rand() % 25));
  };
//  auto str = new char[32];
  for (uint32_t i = 0; i < 32; i++) {
   uuid[i] = randCharFunc();
  }
  //Ensure that the uuid is unique
  bool changed = true;
  uint32_t i = 0;
  while (changed && i < vms.getSize()) {
   changed = false;
   while(vms[i]->uuid == std::string(uuid)) {
    uuid[rand() % 31] = randCharFunc();
    changed = true;
   }
   i = (changed ? 0 : i + 1);
  }
  uuid[32] = '\0';
 }

 Jvm::~Jvm() {
  vms.removeAlloc(this);
  instances.clear();
  for (const auto &l : libraries) {
   removeLibrary(l->path, "");
  }
  libraries.clear();
  delete jvmtiEnv;
  delete jniEnv;
  delete jvmti;
  delete native;
  delete invoke;
  delete[] uuid;
 }

 Library * Jvm::getLibrary(const std::string &path) {
  for (const auto &l : libraries) {
   if (l->path == path) {
    return l;
   }
  }
  return nullptr;
 }

 Library * Jvm::attachLibrary(
  const std::string &rpath,
  const std::string &options,
  FakeJni::LibraryOptions loptions
 ) {
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
 bool Jvm::removeLibrary(const std::string &path, const std::string &options) {
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