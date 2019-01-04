#pragma once

#include "jni.h"

struct FakeJVM {
private:
    JavaVM vm;

public:
    static FakeJVM instance;

    FakeJVM();

    JavaVM* GetFakeJVM() { return &vm; }
};
