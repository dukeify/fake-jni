#include "jni.h"
#include "jvmti.h"

#include "fake_jni/types.h"
#include "fake_jni/jvm.h"
#include "fake_jni/library.h"

#include <iostream>

//Example dynamic JNI linkage
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
 std::cout << "JNI Hello World!" << std::endl;
 return JNI_OK;
}

void JNI_OnUnload(JavaVM *vm, void *reserved) {
 std::cout << "JNI Goodbye World!" << std::endl;
}

//Example dynamic Agent linkage
jint Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
 std::cout << "Cold Start Agent Hello World!" << std::endl;
 return JNI_OK;
}

jint Agent_OnAttach(JavaVM* vm, char* options, void* reserved) {
 std::cout << "Live Attach Agent Hello World!" << std::endl;
 return JNI_OK;
}

void Agent_OnUnload(JavaVM *vm) {
 std::cout << "Agent Goodbye World!" << std::endl;
}

//Alternate dlopen function to open current binary
FakeJni::dlopen_t fakeOpen = [](const char *filename, int flags) -> void * {
 return dlopen(nullptr, flags);
};

//dlclose is currently broken for some reason
FakeJni::dlclose_t fakeClose = [](void *handle) -> int {
 return 0;
};

//fake-jni in action
int main(int argc, char **argv) {
 std::cout << sizeof(JNINativeInterface_) << std::endl;
 //Create a shiny new fake JVM instance
 FakeJni::DefaultJvm vm;

 //Attach this binary as a native library
 //no path to current binary, no options, custom library dl functions
 vm.attachLibrary("", "", {fakeOpen, &dlsym, fakeClose});

 //The invocations below are identical
 //Calls through the JNI (JNIInvokeInterface_)
 vm.getInvokeInterface()->DestroyJavaVM(&vm);
 //Calls through fake-jni
// vm.destroy();
 return 0;
}