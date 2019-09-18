#pragma once

#include "fake-jni/jvm.h"
#include "fake-jni/array.h"

//JNI _jstring and java/lang/String implementation
namespace FakeJni {
 class JString : public _jstring, public JCharArray {
 private:
  static const JString EMPTY_STR;

  JInt length;

 public:
  DEFINE_CLASS_NAME("java/lang/String")
  using cast = typename CX::ExplicitCastGenerator<JString, JCharArray, JClass, JObject>;

  static constexpr JString * const EMPTY = const_cast<JString *>(&EMPTY_STR);

  JString();
  JString(const JString & str);
  explicit JString(JInt size);
  JString(const char * str);
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
 bool JString::operator!=(const T operand) const {
  return !operator==(operand);
 }
}

DECLARE_NATIVE_TYPE(FakeJni::JString)