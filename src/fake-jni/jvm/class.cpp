#include "fake-jni/jvm.h"

#include <vector>

#define _ERROR_ARBITRARY_CLASS \
throw std::runtime_error("FATAL: Cannot construct an arbitrary class with no native backing!");

#define _ALLOC_CLASS_NAME(_name) \
[&]() -> const char * {\
 const auto len = strlen(_name);\
 auto allocName = new char[len + 1];\
 memcpy(allocName, name, len + 1);\
 return allocName;\
}()

namespace FakeJni {
 JClass::JClass(const char * name, uint32_t modifiers) noexcept :
  JObject(),
  constructV([](const JavaVM * const, const char * const, CX::va_list_t&) -> JObject * {
   _ERROR_ARBITRARY_CLASS
  }),
  constructA([](const JavaVM * const, const char * const, const jvalue *) -> JObject * {
   _ERROR_ARBITRARY_CLASS
  }),
  className(_ALLOC_CLASS_NAME(name)),
  isArbitrary(true),
  modifiers(modifiers),
  parent(JObject::descriptor),
  functions{true},
  fields{true}
 {}

 JClass::JClass(const JClass & clazz) noexcept :
  JObject(),
  constructV(clazz.constructV),
  constructA(clazz.constructA),
  className((clazz.isArbitrary ? _ALLOC_CLASS_NAME(clazz.className) : clazz.className)),
  isArbitrary(clazz.isArbitrary),
  modifiers(clazz.modifiers),
  parent(clazz.parent),
  functions{false},
  fields{false}
 {
  for (auto func : clazz.functions) {
   functions.insert(func);
  }
  for (auto field : clazz.fields) {
   fields.insert(field);
  }
 }

 JClass::~JClass() {
  if (isArbitrary) {
   delete[] className;
  }
 }

 uint32_t JClass::getModifiers() const noexcept {
  return modifiers;
 }

 bool JClass::registerMethod(const JMethodID * const mid, bool deallocate) const {
  if (isArbitrary) {
   if (mid->type == JMethodID::MEMBER_FUNC) {
    throw std::runtime_error(
     "FATAL: You cannot register a member function on an arbitrary class that cannot be instantiated!"
    );
   }
  }
  auto& functions_ref = const_cast<PointerList<const JMethodID *>&>(functions);
  for (auto reg_mid : functions_ref) {
   if (*reg_mid == *mid) {
    if (strcmp(reg_mid->getName(), mid->getName()) == 0) {
     if (strcmp(reg_mid->getSignature(), mid->getSignature()) == 0) {
      return false;
     }
    }
   }
  }
  if (deallocate) {
   functions_ref.insert(mid);
  } else {
   functions_ref.insert(mid, nullptr);
  }
  return true;
 }

 bool JClass::unregisterMethod(const JMethodID * const mid) const noexcept {
  auto & functions_ref = const_cast<PointerList<const JMethodID *>&>(functions);
  return functions_ref.end() != functions_ref.erase(mid);
 }

 const JMethodID * JClass::getMethod(const char * sig, const char * name) const noexcept {
  auto& functions_ref = const_cast<PointerList<const JMethodID *>&>(functions);
  for (auto func : functions_ref) {
   if (strcmp(name, func->getName()) == 0) {
    if (strcmp(sig, func->getSignature()) == 0) {
     return func;
    }
   }
  }
  return nullptr;
 }

 const PointerList<const JMethodID *>& JClass::getMethods() const noexcept {
  return functions;
 }

 bool JClass::registerField(JFieldID * const fid, bool deallocate) const noexcept {
  auto& fields_ref = const_cast<PointerList<const JFieldID *>&>(fields);
  for (auto reg_fid : fields_ref) {
   if (*reg_fid == *fid) {
    return false;
   } else if (strcmp(reg_fid->getName(), fid->getName()) == 0) {
    if (strcmp(reg_fid->getSignature(), fid->getSignature()) == 0) {
     return false;
    }
   }
  }
  if (deallocate) {
   fields_ref.insert(fid);
  } else {
   fields_ref.insert(fid, nullptr);
  }
  return true;
 }

 bool JClass::unregisterField(JFieldID * const fid) const noexcept {
  auto & fields_ref = const_cast<PointerList<const JFieldID *>&>(fields);
  return fields_ref.end() != fields_ref.erase(fid);
 }

 const JFieldID * JClass::getField(const char * name) const noexcept {
  const auto * jobjDescriptor = &JObject::descriptor;
  auto * clazz = this;
  const JFieldID * fid = nullptr;
  while (clazz != jobjDescriptor && !fid) {
   auto& fields_ref = const_cast<PointerList<const JFieldID *>&>(clazz->fields);
   for (auto field : fields_ref) {
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
   auto& fields_ref = const_cast<PointerList<const JFieldID *>&>(clazz->fields);
   for (auto field : fields_ref) {
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

 const PointerList<const JFieldID *>& JClass::getFields() const noexcept {
  return fields;
 }

 const char * JClass::getName() const noexcept {
  return className;
 }

 JObject * JClass::newInstance(const JavaVM * const vm, const char * const signature, CX::va_list_t& list) const {
  return constructV(vm, signature, list);
 }

 JObject * JClass::newInstance(const JavaVM * const vm, const char * const signature, const jvalue * values) const {
  return constructA(vm, signature, values);
 }
}

//Allocate JClass descriptor for JClass
DEFINE_NATIVE_DESCRIPTOR(FakeJni::JClass)END_NATIVE_DESCRIPTOR