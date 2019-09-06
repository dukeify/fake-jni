#pragma once

#include "meta.h"

#include <stdexcept>

namespace FakeJni {
 class JFieldID;

 //Template glue code for field registration and access
 namespace _CX {
  //SFINAE templates to generate field accessors
  //Field accessor for mutable member fields
  template<typename...>
  class FieldAccessor;

  //Field accesor for mutable member fields
  template<typename T, typename F>
  class FieldAccessor<F (T::*)> {
  public:
   using type_t = F (T::* const);
   using erased_t = int (AnyClass::* const);

   [[gnu::always_inline]]
   inline static F get(void * inst, erased_t field) {
    return ((T * const)inst)->*((type_t)field);
   }

   [[gnu::always_inline]]
   inline static void set(void * const inst, erased_t field, void * const value) {
    ((T*)inst)->*((type_t)field) = *((F*)value);
   }
  };

  //Field accessor for immutable member fields
  template<typename T, typename F>
  class FieldAccessor<const F (T::*)> {
  public:
   using type_t = const F (T::* const);
   using erased_t = int (AnyClass::* const);

   [[gnu::always_inline]]
   inline static F get(void * inst, erased_t field) {
    return ((T*)inst)->*((type_t)field);
   }

   [[gnu::always_inline]]
   inline static void set(void * const inst, erased_t field, void * const value) {
    std::string error = "Attempted to write to immutable field: '";
    error += typeid(type_t).name();
    error += "'!";
    throw std::runtime_error(error.c_str());
   }
  };

  //Field accessor for mutable non-member fields
  template<typename F>
  class FieldAccessor<F*> {
  public:
   using type_t = F * const;
   using erased_t = void * const;

   [[gnu::always_inline]]
   inline static F get(erased_t field) {
    return *((type_t)field);
   }

   [[gnu::always_inline]]
   inline static void set(erased_t field, void * const value) {
    *((type_t)field) = *((type_t)value);
   }
  };

  //Field accessor for immutable non-member fields
  template<typename F>
  class FieldAccessor<const F*> {
  public:
   using type_t = const F * const;
   using erased_t = void * const;

   [[gnu::always_inline]]
   inline static F get(erased_t field) {
    return *((type_t)field);
   }

   [[gnu::always_inline]]
   inline static void set(erased_t field, void * value) {
    std::string error = "Attempted to write to immutable field: '";
    error +=  typeid(type_t).name();
    error += "'!";
    throw std::runtime_error(error.c_str());
   }
  };
 }
}