#include "fake-jni/jvm.h"
#include "fake-jni/weak.h"

#include <stdexcept>
#include <algorithm>

namespace FakeJni {
 jobjectRefType NativeInterface::getObjectRefType(jobject const jobj) const {
  auto& refs = vm.getReferences();
  if (refs.find(jobj) != refs.end()) {
   return refs[jobj];
  }
  return jobjectRefType::JNIInvalidRefType;
 }

 jobject NativeInterface::newGlobalRef(jobject const jobj) const {
  auto& refs = vm.getReferences();
  if (refs.find(jobj) == refs.end()) {
   vm.getReferences()[jobj] = jobjectRefType::JNIGlobalRefType;
  }
#ifdef FAKE_JNI_DEBUG
  else {
   fprintf(
    vm.getLog(),
    "WARNING: Tried to create global reference to an object with a previously initialized reference type!"
   );
  }
#endif
  return jobj;
 }

 void NativeInterface::deleteGlobalRef(jobject const jobj) const {
  auto& refs = vm.getReferences();
  if (refs.find(jobj) != refs.end()) {
   if (refs[jobj] == jobjectRefType::JNIGlobalRefType) {
    refs.erase(jobj);
   }
  }
#ifdef FAKE_JNI_DEBUG
  else {
   fprintf(
    vm.getLog(),
    "WARNING: Tried to delete non-existent global reference!"
   );
  }
#endif
 }

 jobject NativeInterface::newLocalRef(jobject const jobj) const {
  auto& refs = vm.getReferences();
  if (refs.find(jobj) == refs.end()) {
   vm.getReferences()[jobj] = jobjectRefType::JNILocalRefType;
  }
#ifdef FAKE_JNI_DEBUG
  else {
   fprintf(
    vm.getLog(),
    "WARNING: Tried to create local reference to an object with a previously initialized reference type!"
   );
  }
#endif
  return jobj;
 }

 void NativeInterface::deleteLocalRef(jobject const jobj) const {
  auto& refs = vm.getReferences();
  if (refs.find(jobj) != refs.end()) {
   if (refs[jobj] == jobjectRefType::JNILocalRefType) {
    refs.erase(jobj);
   }
  }
#ifdef FAKE_JNI_DEBUG
  else {
   fprintf(
    vm.getLog(),
    "WARNING: Tried to delete non-existent local reference!"
   );
  }
#endif
 }

 jweak NativeInterface::newWeakGlobalRef(jobject const jobj) const {
  auto& refs = vm.getReferences();
  if (refs.find(jobj) == refs.end()) {
   vm.getReferences()[jobj] = jobjectRefType::JNIWeakGlobalRefType;
  }
#ifdef FAKE_JNI_DEBUG
  else {
   fprintf(
    vm.getLog(),
    "WARNING: Tried to create weak global reference to an object with a previously initialized reference type!"
   );
  }
#endif
  return *jobj;
 }

 void NativeInterface::deleteWeakGlobalRef(jweak const weak) const {
  jobject const jobj = *weak;
  auto& refs = vm.getReferences();
  if (refs.find(jobj) != refs.end()) {
   refs.erase(jobj);
  }
#ifdef FAKE_JNI_DEBUG
  else {
   fprintf(
    vm.getLog(),
    "WARNING: Tried to delete non-existent global weak reference!"
   );
  }
#endif
 }
}