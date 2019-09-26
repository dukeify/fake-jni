#include "fake-jni/jvm.h"

//Non-template members of JFieldID
namespace FakeJni {
 bool JFieldID::operator ==(JFieldID &fid) const noexcept {
  return (isStatic == fid.isStatic)
   && (modifiers == fid.modifiers)
   && (isStatic ? (staticProp == fid.staticProp) : (memberProp == fid.memberProp))
   && (strcmp(name, fid.name) == 0)
   && (strcmp(signature, fid.signature) == 0)
   && (proxyGetFunc == fid.proxyGetFunc)
   && (proxySetFunc == fid.proxySetFunc);
 }

 void JFieldID::set(JObject * const obj, void * const value) {
  if (isStatic) {
   ((void (*)(void *, void *))proxySetFunc)(staticProp, value);
  } else {
   ((void (*)(void *, int (_CX::AnyClass::*), void *))proxySetFunc)(obj, memberProp, value);
  }
 }
}