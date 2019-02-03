#include "agent.h"
#include "fake_jni.h"

#include <iostream>

Agent::Agent(std::string path, AgentOptions const& options)
    : path_(path), options_(options) {
  FakeJVM::instance.addAgent(this);
}

bool Agent::open() {
  handle_ = options_.dlopen_(path_.c_str(), RTLD_LAZY);

  if (!handle_) {
    throw std::runtime_error("Agent failed to load using `dlopen`");
  }

  (void*&) JNI_OnLoad_ = options_.dlsym_(handle_, "JNI_OnLoad");
  (void*&) JNI_OnUnload_ = options_.dlsym_(handle_, "JNI_OnUnload");

  if (!JNI_OnLoad_ || !JNI_OnUnload_) {
    throw std::runtime_error("JNI_OnLoad or JNI_OnUnload not found");
  }

  return JNI_OnLoad_(FakeJVM::instance.getFakeJVM(), nullptr) > JNI_VERSION_1_8;
}

bool Agent::close() {
  JNI_OnUnload_(FakeJVM::instance.getFakeJVM(), nullptr);
  return options_.dlclose_(handle_) ? false : true;
}
