#pragma once

#include "fake-jni/jvm.h"
#include "fake-jni/array.h"

//JNI _jstring and java/lang/String implementation
namespace FakeJni {
 class JString : public _jstring, public JCharArray {
 private:
  static const JString EMPTY_STR;

 public:
  DEFINE_CLASS_NAME("java/lang/String")
  using cast = typename CX::ExplicitCastGenerator<JString, JCharArray, JClass, JObject>;

  static constexpr JString * const EMPTY = const_cast<JString *>(&EMPTY_STR);

  const JInt length;

  JString();
  JString(const JString & str);
  explicit JString(JInt size);
  JString(const char * str);

  bool operator==(const JString & str) const;
  bool operator==(const char * str) const;
  bool operator==(const std::string & str) const;
  JString& operator=(const JString& str) const;
  virtual JInt getLength() const override;
 };
}

DECLARE_NATIVE_TYPE(FakeJni::JString)