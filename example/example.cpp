#include <dlfcn.h>
#include <linux/limits.h>
#include <unistd.h>
#include <iostream>

#include "fake_jni.h"
#include "agent.h"

using std::string;

string GetCurrentPath() {
   char buf[PATH_MAX];
   return (getcwd(buf, sizeof(buf)) ? buf : "");
}

int main(int argc, char* argv[]) {
    Agent agent (GetCurrentPath() + "/libexample_jni_agent.so", { &dlopen, &dlsym, &dlclose });
    FakeJVM::instance.close();
    return 0;
}
