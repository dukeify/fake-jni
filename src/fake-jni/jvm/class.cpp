#include "jni.h"

#include "fake-jni/jvm.h"

#include <vector>

namespace FakeJni {
 bool JClass::registerMethod(JMethodID * const mid) const noexcept {
  auto functions_ptr = const_cast<AllocStack<JMethodID *> *>(&functions);
  for (uint32_t i = 0; i < functions_ptr->getSize(); i++) {
   if (*(*functions_ptr)[i] == *mid) {
    return false;
   }
  }
  functions_ptr->pushAlloc(mid);
  return true;
 }

 bool JClass::unregisterMethod(FakeJni::JMethodID * const mid) const noexcept {
  auto functions_ptr = const_cast<AllocStack<JMethodID *> *>(&functions);
  for (unsigned int i = 0; i < functions_ptr->getSize(); i++) {
   if ((*functions_ptr)[i] == mid) {
    functions_ptr->removeAlloc(mid);
    return true;
   }
  }
  return false;
 }

 JMethodID * JClass::getMethod(const char * sig, const char * name) const noexcept {
  auto functions_ptr = const_cast<AllocStack<JMethodID *> *>(&functions);
  for (unsigned int i = 0; i < functions_ptr->getSize(); i++) {
   const auto& func = (*functions_ptr)[i];
   if (strcmp(name, func->getName()) == 0) {
    if (strcmp(sig, func->getSignature()) == 0) {
     return func;
    }
   }
  }
  return nullptr;
 }

 const AllocStack<JMethodID *>& JClass::getMethods() const noexcept {
  return functions;
 }

 bool JClass::registerField(JFieldID * const fid) const noexcept {
  auto fields_ptr = const_cast<AllocStack<JFieldID *> *>(&fields);
  for (uint32_t i = 0; i < fields_ptr->getSize(); i++) {
   if (*(*fields_ptr)[i] == *fid) {
    return false;
   }
  }
  fields_ptr->pushAlloc(fid);
  return true;
 }

 bool JClass::unregisterField(FakeJni::JFieldID * const fid) const noexcept {
  auto fields_ptr = const_cast<AllocStack<JFieldID *> *>(&fields);
  for (unsigned int i = 0; i < fields_ptr->getSize(); i++) {
   if ((*fields_ptr)[i] == fid) {
    fields_ptr->removeAlloc(fid);
    return true;
   }
  }
  return false;
 }

 JFieldID * JClass::getField(const char * name) const noexcept {
  auto fields_ptr = const_cast<AllocStack<JFieldID *> *>(&fields);
  for (unsigned int i = 0; i < fields_ptr->getSize(); i++) {
   const auto& field = (*fields_ptr)[i];
   if (strcmp(name, field->getName()) == 0) {
    return field;
   }
  }
  return nullptr;
 }

 const AllocStack<JFieldID *>& JClass::getFields() const noexcept {
  return fields;
 }

 const char * JClass::getName() const noexcept {
  return className;
 }

 JObject * JClass::newInstance(JavaVM * const vm, const char * const signature, va_list list) const {
  return constructV(vm, signature, list);
 }

 JObject * JClass::newInstance(JavaVM * const vm, const char * const signature, const jvalue * const values) const {
  return constructA(vm, signature, values);
 }
}

//Allocate JClass descriptor for JClass
DEFINE_NATIVE_DESCRIPTOR(FakeJni::JClass)END_NATIVE_DESCRIPTOR