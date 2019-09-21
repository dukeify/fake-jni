#include "jni.h"

#include "fake-jni/jvm.h"
#include "fake-jni/string.h"

#include <cx/unsafe.h>

#include <stdexcept>

namespace FakeJni {
 //TODO implement
 jstring NativeInterface::newString(const jchar * chars, jsize size) const {
  return nullptr;
 }

 jsize NativeInterface::getStringLength(jstring jstr) const {
  return CX::union_cast<JString *>(jstr)->getLength();
 }

 jchar * NativeInterface::getStringChars(jstring jstr, jboolean * copy) const {
  return CX::union_cast<JString *>(jstr)->getArray();
 }

 void NativeInterface::releaseStringChars(jstring jstr, const jchar * chars) const {}

 jstring NativeInterface::newStringUTF(const char *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newStringUTF' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jsize NativeInterface::getStringUTFLength(jstring) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStringUTFLength' is unimplemented!");
  return 0;
 }

 char* NativeInterface::getStringUTFChars(jstring jstr, jboolean * copy) const {}

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