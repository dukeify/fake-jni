#pragma once

#include "jni.h"
#include "agent.h"

#include <vector>

class FakeJVM {
private:
    JavaVM vm_;
    std::vector<Agent*> agents_;

public:
    static FakeJVM instance;

    FakeJVM();

    JavaVM* getFakeJVM() { return &vm_; }

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
