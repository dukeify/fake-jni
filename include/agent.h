#pragma once

#include "jni.h"

#include <dlfcn.h>

#include <stdexcept>
#include <string>

// TODO(meme) make this not gross
struct AgentOptions {
  void* (*dlopen_)(const char* filename, int flags);
  void* (*dlsym_)(void* handle, const char* symbol);
  int (*dlclose_)(void* handle);

  AgentOptions(void* (*dlopen_)(const char*, int),
               void* (*dlsym_)(void*, const char*),
               int (*dlclose_)(void*)) :
      dlopen_(dlopen_),
      dlsym_(dlsym_), dlclose_(dlclose_) {}
};

class Agent {
private:
  std::string path_;
  AgentOptions const& options_;

  void* handle_;
  jint (*JNI_OnLoad_)(JavaVM* vm, void* reserved);
  void (*JNI_OnUnload_)(JavaVM* vm, void* reserved);

public:
  Agent(std::string path, AgentOptions const& options);

  bool open();

  bool close();
};
