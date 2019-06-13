#include <iostream>

#include "jni.h"

//Example dynamic JNI linkage
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
 std::cout << "JNI Hello World!" << std::endl;
 return JNI_OK;
}

void JNI_OnUnload(JavaVM *vm, void *reserved) {
 std::cout << "JNI Goodbye World!" << std::endl;
}