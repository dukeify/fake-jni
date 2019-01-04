#include <dlfcn.h>
#include <linux/limits.h>
#include <unistd.h>
#include <iostream>

#include "fake_jvm.h"

using std::string;

string GetCurrentPath() {
   char buf[PATH_MAX];
   return (getcwd(buf, sizeof(buf)) ? buf : "");
}

static jint (*JNI_OnLoad_)(JavaVM *vm, void *reserved);
static void (*JNI_OnUnload_)(JavaVM *vm, void *reserved);

int main(int argc, char* argv[]) {
    void *exampleJniAgent = dlopen((GetCurrentPath() + "/libexample_jni_agent.so").c_str(), RTLD_LAZY);

    (void*&) JNI_OnLoad_ = dlsym(exampleJniAgent, "JNI_OnLoad");
    (void*&) JNI_OnUnload_ = dlsym(exampleJniAgent, "JNI_OnUnload");

    JavaVM* vm = FakeJVM::instance.GetFakeJVM();
    JNI_OnLoad_(vm, nullptr);
    JNI_OnUnload_(vm, nullptr);

    dlclose(exampleJniAgent);
    return 0;
}
