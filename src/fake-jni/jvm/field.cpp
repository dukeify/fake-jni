#include "fake-jni/jvm.h"

#define _JFIELDID_PROXY_CMP \
(proxyGetFunc == fid.proxyGetFunc) && (proxySetFunc == fid.proxySetFunc)

//Non-template members of JFieldID
namespace FakeJni {
 JFieldID::JFieldID(const JFieldID * fid, bool dealloc) :
  _jfieldID(),
  type(COMPOSED_PROP),
  modifiers(fid->modifiers),
  name(fid->name),
  signature(fid->signature),
  composed(fid),
  dealloc(dealloc),
  //TODO remove these
  proxyGetFunc(nullptr),
  proxySetFunc(nullptr),
  isArbitrary(false)
 {}

 JFieldID::JFieldID(v_get_func_t get, v_set_func_t set, const char * name, const char * signature, uint32_t modifiers) noexcept :
  _jfieldID(),
  type(CALLBACK_PROP),
  modifiers(modifiers),
  name(_INTERNAL_ARBITRARY_ALLOC_STR(name)),
  signature(_INTERNAL_ARBITRARY_ALLOC_STR(signature)),
  proxyGetFunc((void (*)())get),
  proxySetFunc((void (*)())set),
  isArbitrary(true)
 {}

 JFieldID::JFieldID(CX::Lambda<void *()> get, CX::Lambda<void(void *)> set, const char *name, const char *signature, uint32_t modifiers) noexcept :
  _jfieldID(),
  type(STL_CALLBACK_PROP),
  modifiers(modifiers),
  name(_INTERNAL_ARBITRARY_ALLOC_STR(name)),
  signature(_INTERNAL_ARBITRARY_ALLOC_STR(signature)),
  arbitraryGet(CX::union_cast<decltype(arbitraryGet)>(std::move(get))),
  arbitrarySet(CX::union_cast<decltype(arbitraryGet)>(std::move(set))),
  isArbitrary(true)
 {}

 JFieldID::~JFieldID() {
  switch(type) {
   case COMPOSED_PROP: {
    if (dealloc) {
     delete composed;
    }
    break;
   }
   default: break;
  }
  if (isArbitrary) {
   delete[] name;
   delete[] signature;
  }
 }

 const char * JFieldID::getName() const noexcept {
  switch(type) {
   case COMPOSED_PROP: return composed->getName();
   default: return name;
  }
 }

 const char * JFieldID::getSignature() const noexcept {
  switch(type) {
   case COMPOSED_PROP: return composed->getSignature();
   default: return signature;
  }
 }

 uint32_t JFieldID::getModifiers() const noexcept {
  switch(type) {
   case COMPOSED_PROP: return composed->getModifiers();
   default: return modifiers;
  }
 }

 bool JFieldID::operator ==(const JFieldID &fid) const noexcept {
  switch(type) {
   case COMPOSED_PROP: return composed->operator==(fid);
   default: {
    const auto predicate = (type == fid.type)
     && (modifiers == fid.modifiers)
     && (strcmp(name, fid.name) == 0)
     && (strcmp(signature, fid.signature) == 0)
     && (isArbitrary == fid.isArbitrary);
    if (predicate) {
     switch (type) {
      case STATIC_PROP: return _JFIELDID_PROXY_CMP && (staticProp == fid.staticProp);
      case MEMBER_PROP: return _JFIELDID_PROXY_CMP && (memberProp == fid.memberProp);
      case CALLBACK_PROP: return _JFIELDID_PROXY_CMP;
      case STL_CALLBACK_PROP: {
       return (arbitraryGet == fid.arbitraryGet) && (arbitrarySet == fid.arbitrarySet);
      }
      default: break;
     }
    }
    return false;
   }
  }
 }

 const JFieldID * JFieldID::findVirtualMatch(const JClass * clazz) const noexcept {
  switch(type) {
   case COMPOSED_PROP: return composed->findVirtualMatch(clazz);
   default: {
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
  }
 }

 jvalue JFieldID::get(const JavaVM * vm, JObject * obj) const {
  switch(type) {
   case COMPOSED_PROP: return composed->get(vm, obj);
   default: return CX::union_cast<jvalue>(get<jvalue>(obj));
  }
 }

 void JFieldID::set(const JavaVM * vm, JObject * const obj, void * const value) const {
  switch(type) {
   case COMPOSED_PROP: return composed->set(vm, obj, value);
   default: {
    auto clazz = &obj->getClass();
    const JFieldID * fid = this;
    if (clazz != &JClass::descriptor) {
     fid = findVirtualMatch(clazz);
    }
    if (fid) {
     switch (fid->type) {
      case STATIC_PROP: {
       _JFIELDID_STATIC_CHECK(
        ((void (*)(void *, void *))fid->proxySetFunc)(staticProp, value);
        return;
       )
      }
      case MEMBER_PROP: {
       _JFIELDID_STATIC_CHECK(
        ((void (*)(void *, int (_CX::AnyClass::*), void *))fid->proxySetFunc)(obj, memberProp, value);
        return;
       )
      }
      case CALLBACK_PROP: {
       _JFIELDID_STATIC_CHECK(
        ((void (*)(void *, void *))proxySetFunc)(obj, value);
        return;
       )
      }
      case STL_CALLBACK_PROP: {
       _JFIELDID_STATIC_CHECK(
        CX::union_cast<CX::Lambda<void (void *)>>(arbitrarySet)(value);
        return;
       )
      }
      default: break;
     }
    }
    throw std::runtime_error(
     "FATAL: '"
     + std::string(clazz->getName())
     + "' does not contain or inherit any fields matching '"
     + name
     + "' -> '"
     + signature
     + "'!"
    );
   }
  }
 }
}