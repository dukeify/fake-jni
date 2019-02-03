#pragma once

#include "agent.h"
#include "jni.h"

#include <vector>

class FakeJVM {
private:
  JavaVM vm_;
  JNIInvokeInterface_ invoke_;

  JNIEnv env_;
  JNINativeInterface_ native_;

  std::vector<Agent*> agents_;

public:
  static FakeJVM instance;

  FakeJVM();

  JavaVM* getFakeJVM() {
    return &vm_;
  }

  JNIEnv* getFakeEnv() {
    return &env_;
  }

  void addAgent(Agent* agent) {
    agents_.push_back(agent);
    agent->open();
  }

  void close() {
    for (auto& agent : agents_) {
      agent->close();
    }
  }
};
