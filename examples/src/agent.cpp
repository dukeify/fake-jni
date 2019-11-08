#include <iostream>

#include "jni.h"

extern "C" {
 //Example dynamic Agent linkage
 JNIEXPORT
 jint Agent_OnLoad(JavaVM * vm, char * options, void * reserved) {
  std::cout << "Cold Start Agent Hello World!" << std::endl;
  return JNI_OK;
 }

 JNIEXPORT
 jint Agent_OnAttach(JavaVM * vm, char * options, void * reserved) {
  std::cout << "Live Attach Agent Hello World!" << std::endl;
  return JNI_OK;
 }

 JNIEXPORT
 void Agent_OnUnload(JavaVM * vm) {
  std::cout << "Agent Goodbye World!" << std::endl;
 }
}