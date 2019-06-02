#include <dlfcn.h>
#ifdef __APPLE__
#include <sys/syslimits.h>
#else
#include <linux/limits.h>
#endif
#include <unistd.h>
#include <iostream>

#include "agent.h"
#include "fake_jni.h"

using std::string;

string GetCurrentPath() {
   char buf[PATH_MAX];
   return (getcwd(buf, sizeof(buf)) ? buf : "");
}

int main(int argc, char* argv[]) {
  Agent agent(GetCurrentPath() + "/libexample_jni_agent.so",
              {&dlopen, &dlsym, &dlclose});
  FakeJVM::instance.close();
  return 0;
}