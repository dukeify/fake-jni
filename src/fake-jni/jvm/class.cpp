#include "fake-jni/jvm.h"

#include <vector>

#define _ERROR_ARBITRARY_CLASS \
throw std::runtime_error("FATAL: Cannot construct an arbitrary class with no native backing!");

namespace FakeJni {
 JClass::JClass(const char *name, uint32_t modifiers) noexcept :
  JObject(),
  constructV([](const JavaVM * const, const char * const, va_list) -> JObject * {
   _ERROR_ARBITRARY_CLASS
  }),
  constructA([](const JavaVM * const, const char * const, const jvalue *) -> JObject * {
   _ERROR_ARBITRARY_CLASS
  }),
  isArbitrary(true),
  className(name),
  modifiers(modifiers),
  parent(JObject::descriptor),
  functions{true},
  fields{true}
 {}

 JClass::JClass(const JClass& clazz) noexcept :
  JObject(),
  constructV(clazz.constructV),
  constructA(clazz.constructA),
  isArbitrary(clazz.isArbitrary),
  className(clazz.className),
  modifiers(clazz.modifiers),
  parent(clazz.parent),
  functions{false},
  fields{false}
 {
  auto size = clazz.functions.getSize();
  for (unsigned int i = 0; i < size; i++) {
   functions.pushAlloc(clazz.functions[i]);
  }
  size = clazz.fields.getSize();
  for (unsigned int i = 0; i < size; i++) {
   fields.pushAlloc(clazz.fields[i]);
  }
 }

 bool JClass::registerMethod(const JMethodID * const mid) const {
  if (isArbitrary) {
   if (mid->type == JMethodID::MEMBER_FUNC) {
    throw std::runtime_error(
     "FATAL: You cannot register a member function on an arbitrary class that cannot be instantiated!"
    );
   }
  }
  auto& functions_ref = const_cast<AllocStack<const JMethodID *>&>(functions);
  const auto size = functions_ref.getSize();
  for (uint32_t i = 0; i < size; i++) {
   const auto reg_mid = functions_ref[i];
   if (*reg_mid == *mid) {
    if (strcmp(reg_mid->getName(), mid->getName()) == 0) {
     if (strcmp(reg_mid->getSignature(), mid->getSignature()) == 0) {
      return false;
     }
    }
   }
  }
  functions_ref.pushAlloc([](void *mid) { delete (JMethodID *)mid; }, mid);
  return true;
 }

 bool JClass::unregisterMethod(const JMethodID * const mid) const noexcept {
  return const_cast<AllocStack<const JMethodID *>&>(functions).removeAlloc(mid);
 }

 const JMethodID * JClass::getMethod(const char * sig, const char * name) const noexcept {
  auto& functions_ref = const_cast<AllocStack<const JMethodID *>&>(functions);
  const auto size = functions_ref.getSize();
  for (unsigned int i = 0; i < size; i++) {
   const auto& func = functions_ref[i];
   if (strcmp(name, func->getName()) == 0) {
    if (strcmp(sig, func->getSignature()) == 0) {
     return func;
    }
   }
  }
  return nullptr;
 }

 const AllocStack<const JMethodID *>& JClass::getMethods() const noexcept {
  return functions;
 }

 bool JClass::registerField(JFieldID * const fid) const noexcept {
  auto& fields_ref = const_cast<AllocStack<const JFieldID *>&>(fields);
  const auto size = fields_ref.getSize();
  for (uint32_t i = 0; i < size; i++) {
   const auto reg_fid = fields_ref[i];
   if (*reg_fid == *fid) {
    return false;
   } else if (strcmp(reg_fid->getName(), fid->getName()) == 0) {
    if (strcmp(reg_fid->getSignature(), fid->getSignature()) == 0) {
     return false;
    }
   }
  }
  fields_ref.pushAlloc([](void *fid) { delete (JFieldID *)fid; }, fid);
  return true;
 }

 bool JClass::unregisterField(JFieldID * const fid) const noexcept {
  return const_cast<AllocStack<const JFieldID *>&>(fields).removeAlloc(fid);
 }

 const JFieldID * JClass::getField(const char * name) const noexcept {
  const auto * jobjDescriptor = &JObject::descriptor;
  auto * clazz = this;
  const JFieldID * fid = nullptr;
  while (clazz != jobjDescriptor && !fid) {
   auto& fields_ref = const_cast<AllocStack<const JFieldID *>&>(clazz->fields);
   const auto size = fields_ref.getSize();
   for (unsigned int i = 0; i < size; i++) {
    const auto& field = fields_ref[i];
    if (strcmp(name, field->getName()) == 0) {
     fid = field;
     break;
    }
   }
   clazz = &clazz->parent;
  }
  return fid;
 }

 const JFieldID * JClass::getField(const char * sig, const char * name) const noexcept {
  const auto * jobjDescriptor = &JObject::descriptor;
  auto * clazz = this;
  const JFieldID * fid = nullptr;
  while (clazz != jobjDescriptor && !fid) {
   auto& fields_ref = const_cast<AllocStack<const JFieldID *>&>(clazz->fields);
   const auto size = fields_ref.getSize();
   for (unsigned int i = 0; i < size; i++) {
    const auto& field = fields_ref[i];
    if (strcmp(sig, field->getSignature()) == 0) {
     if (strcmp(name, field->getName()) == 0) {
      fid = field;
      break;
     }
    }
   }
   clazz = &clazz->parent;
  }
  return fid;
 }

 const AllocStack<const JFieldID *>& JClass::getFields() const noexcept {
  return fields;
 }

 const char * JClass::getName() const noexcept {
  return className;
 }

 JObject * JClass::newInstance(const JavaVM * const vm, const char * const signature, va_list list) const {
  return constructV(vm, signature, list);
 }

 JObject * JClass::newInstance(const JavaVM * const vm, const char * const signature, const jvalue * const values) const {
  return constructA(vm, signature, values);
 }
}

//Allocate JClass descriptor for JClass
DEFINE_NATIVE_DESCRIPTOR(FakeJni::JClass)END_NATIVE_DESCRIPTOR