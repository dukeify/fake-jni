#include <iostream>

#include "jni.h"

extern "C" {
 //Example dynamic JNI linkage
 JNIEXPORT
 jint JNI_OnLoad(JavaVM * vm, void * reserved) {
  std::cout << "JNI Hello World!" << std::endl;
  return JNI_OK;
 }

 JNIEXPORT
 void JNI_OnUnload(JavaVM * vm, void * reserved) {
  std::cout << "JNI Goodbye World!" << std::endl;
 }
}