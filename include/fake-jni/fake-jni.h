#pragma once

#include <functional>

#include "fake-jni/jvm.h"
#include "fake-jni/array.h"
#include "fake-jni/string.h"
#include "fake-jni/throwable.h"
#include "fake-jni/weak.h"

//General utility functions
namespace FakeJni {
 namespace _CX {
  using main_method_t = void (JArray<JString *>);
 }

 JClass * createDummyClass(const Jvm & vm);

 //Utility functions for easily registering a native main method
 void createMainMethod(const Jvm & vm, _CX::main_method_t * main, JClass * clazz = nullptr);
 void createMainMethod(const Jvm & vm, std::function<_CX::main_method_t> main, JClass * clazz = nullptr);
}