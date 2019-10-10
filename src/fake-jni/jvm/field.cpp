#include "fake-jni/jvm.h"

#define _JFIELDID_SET \
if (isStatic) {\
 ((void (*)(void *, void *))fid->proxySetFunc)(staticProp, value);\
} else {\
 ((void (*)(void *, int (_CX::AnyClass::*), void *))fid->proxySetFunc)(obj, memberProp, value);\
}

//Non-template members of JFieldID
namespace FakeJni {
 JFieldID::JFieldID(v_get_func_t get, v_set_func_t set, const char * name, const char * signature, uint32_t modifiers) noexcept :
  _jfieldID(),
  isStatic(false),
  modifiers(modifiers),
  name(name),
  signature(signature),
  proxyGetFunc((void (*)())get),
  proxySetFunc((void (*)())set),
  isArbitrary(true)
 {}

 const JFieldID * JFieldID::findVirtualMatch(const JClass * clazz) const noexcept {
  const auto * jobjDescriptor = JObject::getDescriptor();
  const JFieldID * fieldDescriptor = clazz->getField(signature, name);
  if (!fieldDescriptor) {
   clazz = &clazz->parent;
   while (clazz != jobjDescriptor) {
    for (auto& fid : clazz->getFields()) {
     if (strcmp(fid->getName(), name) == 0) {
      if (strcmp(fid->getSignature(), signature) == 0) {
       fieldDescriptor = fid;
       break;
      }
     }
    }
    clazz = &clazz->parent;
   }
  }
  return fieldDescriptor;
 }

 bool JFieldID::operator ==(const JFieldID &fid) const noexcept {
  return (isStatic == fid.isStatic)
   && (modifiers == fid.modifiers)
   && (isStatic ? (staticProp == fid.staticProp) : (memberProp == fid.memberProp))
   && (strcmp(name, fid.name) == 0)
   && (strcmp(signature, fid.signature) == 0)
   && (proxyGetFunc == fid.proxyGetFunc)
   && (proxySetFunc == fid.proxySetFunc)
   && (isArbitrary == fid.isArbitrary);
 }

 void JFieldID::set(JObject * const obj, void * const value) const {
  auto& clazz = obj->getClass();
  auto * fid = findVirtualMatch(&clazz);
  if (fid) {
   if (isArbitrary) {
    ((void (*)(void *, void *))proxySetFunc)(obj, value);
   } else {
    //If the field is static, it is not inherited from the base
    if ((modifiers & STATIC) == STATIC) {
     if (fid == this) {
      _JFIELDID_SET
     }
    } else {
     _JFIELDID_SET
    }
   }
  }
  throw std::runtime_error(
   "FATAL: '"
    + std::string(clazz.getName())
    + "' does not contain or inherit any fields matching '"
    + name
    + "' -> '"
    + signature
    + "'!"
  );
 }
}