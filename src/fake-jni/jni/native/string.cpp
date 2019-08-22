#include "jni.h"
#include "fake-jni/jvm.h"

#include <stdexcept>

namespace FakeJni {
//TODO implement
 jstring NativeInterface::newString(const jchar *chars, jsize size) const {

  return nullptr;
 }

//TODO implement
 jsize NativeInterface::getStringLength(jstring) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStringLength' is unimplemented!");
  return 0;
 }

//TODO implement
 jchar* NativeInterface::getStringChars(jstring, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStringChars' is unimplemented!");
  return nullptr;
 }

//TODO implement
 void NativeInterface::releaseStringChars(jstring, const jchar *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releaseStringChars' is unimplemented!");
 }

//TODO implement
 jstring NativeInterface::newStringUTF(const char *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newStringUTF' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jsize NativeInterface::getStringUTFLength(jstring) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStringUTFLength' is unimplemented!");
  return 0;
 }

//TODO implement
 char* NativeInterface::getStringUTFChars(jstring, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStringUTFChars' is unimplemented!");
  return nullptr;
 }

//TODO implement
 void NativeInterface::releaseStringUTFChars(jstring, const char *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releaseStringUTFChars' is unimplemented!");
 }

//TODO implement
 void NativeInterface::getStringRegion(jstring, jsize, jsize, jchar *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStringRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::getStringUTFRegion(jstring, jsize, jsize, char *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStringUTFRegion' is unimplemented!");
 }

//TODO implement
 jchar* NativeInterface::getStringCritical(jstring, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStringCritical' is unimplemented!");
  return nullptr;
 }

//TODO implement
 void NativeInterface::releaseStringCritical(jstring, const jchar *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releaseStringCritical' is unimplemented!");
 }
}