#pragma once

#include <cstring>
#include <functional>

namespace FakeJni {
 class JObject: public _jobject {
 public:
  explicit JObject(const JObject&) = delete;

  //nullptr is the default JObject instance descriptor
  constexpr JObject() = default;

  virtual ~JObject() = default;

  virtual const JClass * getClass() noexcept {
   return nullptr;
  }
 };

 DEFINE_JNI_TYPE(JObject, Ljava/lang/Object;)
}