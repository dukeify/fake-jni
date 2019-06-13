#pragma once

#include <iostream>

//Internal macro
#define _ASSERT_FIELD_JNI_COMPLIANCE \
static_assert(\
 std::is_base_of<JObject, T>::value || _CX::JniTypeBase<T>::isRegisteredType,\
 "Field type is not a valid JNI type!"\
);\
static_assert(\
 _CX::ComponentTypeResolver<T>::indirectionCount == 0U,\
 "Registered field types cannot be pointers or references to a JNI type!"\
);

namespace FakeJni {
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
    //TODO log that this is not possible because the field is immutable
    std::cerr << "Attempted to write to immutable field!" << std::endl;
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
    //TODO log that this is not possible because the field is immutable
    std::cerr << "Attempted to write to immutable field!" << std::endl;
   }
  };
 }

 //JNI base
 struct _jfieldID {};

 //fake-jni implementation
 class JFieldID: public _jfieldID {
 private:
  using staticProp_t = void * const;
  using memberProp_t = int (_CX::AnyClass::* const);

  const bool isStatic;
  const uint32_t modifiers;
  union {
   staticProp_t staticProp;
   memberProp_t memberProp;
  };
  const char
   * const name,
   * const signature;

  void
  (* const proxyGetFunc)(),
  (* const proxySetFunc)();

 public:
  //TODO ensure that these are correct
  //FIELD_MODIFIERS = 223;
  enum Modifiers: uint32_t {
   PUBLIC = 1,
   PRIVATE = 2,
   PROTECTED = 4,
   STATIC = 8,
   FINAL = 16,
   VOLATILE = 64,
   TRANSIENT = 128
  };

  template<typename T, typename M>
  JFieldID(T M::* const member, const char * const name, const uint32_t modifiers) noexcept :
   _jfieldID(),
   isStatic(false),
   modifiers(modifiers),
   memberProp((memberProp_t)member),
   name(name),
   signature(_CX::JniTypeBase<T>::signature),
   proxyGetFunc((void (*)())&_CX::FieldAccessor<T (M::*)>::get),
   proxySetFunc((void (*)())&_CX::FieldAccessor<T (M::*)>::set)
  {
   _ASSERT_FIELD_JNI_COMPLIANCE
   static_assert(std::is_base_of<JObject, M>::value, "Encapsulating class is not derived from JObject!");
  }

  template<typename T>
  JFieldID(T * const staticMember, const char * const name, const uint32_t modifiers) noexcept :
   _jfieldID(),
   isStatic(true),
   modifiers(modifiers),
   staticProp((staticProp_t)staticMember),
   name(name),
   signature(_CX::JniTypeBase<T>::signature),
   proxyGetFunc((void (*)())&_CX::FieldAccessor<T*>::get),
   proxySetFunc((void (*)())&_CX::FieldAccessor<T*>::set)
  {
   _ASSERT_FIELD_JNI_COMPLIANCE
  }

  bool operator ==(JFieldID &fid) const noexcept {
   return (isStatic == fid.isStatic)
    && (modifiers == fid.modifiers)
    && (isStatic ? (staticProp == fid.staticProp) : (memberProp == fid.memberProp))
    && (strcmp(name, fid.name) == 0)
    && (strcmp(signature, fid.signature) == 0)
    && (proxyGetFunc == fid.proxyGetFunc)
    && (proxySetFunc == fid.proxySetFunc);
  }

  const char * getName() const noexcept {
   return name;
  }

  const char * getSignature() const noexcept {
   return signature;
  }

  uint32_t getModifiers() const noexcept {
   return modifiers;
  }

  template<typename T>
  T get(JObject * const obj) {
   if (isStatic) {
    return ((T (*)(void *))proxyGetFunc)(staticProp);
   } else {
    return ((T (*)(void *, void *))proxyGetFunc)(obj, memberProp);
   }
  }

  void set(JObject * const obj, void * const value) {
   if (isStatic) {
    ((void (*)(void *, void *))proxySetFunc)(staticProp, value);
   } else {
    ((void (*)(void *, int (_CX::AnyClass::*), void *))proxySetFunc)(obj, memberProp, value);
   }
  }
 };
}