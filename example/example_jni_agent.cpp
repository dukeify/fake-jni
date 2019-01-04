#include "jni.h"

#include <iostream>

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    std::cout << "JNI_OnLoad\n";

    return JNI_VERSION_1_8;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved) {
    std::cout << "JNI_OnUnload\n";
}
