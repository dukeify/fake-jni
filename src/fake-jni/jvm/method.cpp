#include "jni.h"
#include "fake-jni/jvm.h"

//Non-template members of JMethodID
namespace FakeJni {
 bool JMethodID::operator ==(JMethodID &mid) const noexcept {
  return (name == mid.name)
   && (strcmp(signature, mid.signature) == 0)
   && (isStatic == mid.isStatic)
   && (modifiers == mid.modifiers)
   && (isStatic ? (memberFunc == mid.memberFunc) : (staticFunc == mid.staticFunc))
   && (proxyFuncV == mid.proxyFuncV)
   && (proxyFuncA == mid.proxyFuncA);
 }
}