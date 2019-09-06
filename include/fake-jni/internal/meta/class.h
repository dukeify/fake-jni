#pragma once

#include "jni.h"

#include <initializer_list>

//Template glue code for native class registration
namespace FakeJni::_CX {
 class ClassDescriptorElement;

 //TODO detect if function is virtual: if any derived classes, of previously defined native classes, contain
 // a virtual override for the base, then register a new java function association for the derived class and
 // the respective function signature and classpath
 template<typename T>
 class JClassBreeder final {
 private:
  template<typename A>
  using constructor_func_t = JObject * (*)(JavaVM * vm, const char * signature, A args);

  static constexpr const auto deallocator = &_CX::Deallocator<T>::deallocate;
  //TODO default cast route

 public:
  const std::initializer_list<ClassDescriptorElement> descriptorElements;

  constexpr JClassBreeder(decltype(descriptorElements) descriptorElements) noexcept;

  template<typename A>
  static constexpr constructor_func_t<A> constructorPredicate() noexcept;

  static constexpr void assertNameCompliance() noexcept;
 };
}