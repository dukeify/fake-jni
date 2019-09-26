#pragma once

#include "fake-jni/jvm.h"
#include "fake-jni/array.h"

//JNI java/lang/String implementation
namespace FakeJni {
 class JString : public JCharArray {
 private:
  static const JString EMPTY_STR;

  JInt length;

 public:
  DEFINE_CLASS_NAME("java/lang/String")
//  using cast = typename CX::ExplicitCastGenerator<JString, JCharArray, JClass, JObject>;

  template<typename T>
  operator T() const;

  static constexpr JString * const EMPTY = const_cast<JString *>(&EMPTY_STR);

  JString();
  JString(const JString & str);
  explicit JString(JInt size);
  JString(const char * str);
  JString(const JChar * str, JInt size);
  virtual ~JString() = default;

  bool operator==(const JString & str) const;
  bool operator==(const char * str) const;
  bool operator==(const std::string & str) const;
  template<typename T>
  bool operator!=(T operand) const;
  JString& operator=(const JString& str) const;
  JInt getLength() const;
 };

 template<typename T>
 JString::operator T() const {
  using component_t = typename CX::ComponentTypeResolver<T>::type;
  constexpr const auto
   downcast = __is_base_of(JString, component_t),
   upcast = __is_base_of(component_t, JString),
   jnicast = CX::MatchAny<component_t, _jobject, _jstring>::value;
  static_assert(
   downcast || upcast || jnicast,
   "JString can only be upcast, downcast, or converted to _jobject or _jstring!"
  );
  auto ptr = const_cast<JString *>(this);
  if constexpr(upcast || downcast) {
   return (T&)*ptr;
  } else if constexpr (jnicast) {
   return CX::union_cast<T>(this)();
  }
 }

 template<typename T>
 bool JString::operator!=(const T operand) const {
  return !operator==(operand);
 }
}

_DEFINE_JNI_CONVERSION_OPERATOR_(FakeJni::JString, jstring)

DEFINE_JNI_TYPE(_jstring, "java/lang/String")
DECLARE_NATIVE_TYPE(FakeJni::JString)