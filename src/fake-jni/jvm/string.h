#pragma once

#include <string>

namespace FakeJni {
 class JString: public _jstring, public JCharArray, public virtual NativeObject<JString> {
 public:
  DEFINE_CLASS_NAME("java/lang/String")
  using cast = typename _CX::ExplicitCastGenerator<JString, JCharArray, JClass, JObject>;

  const size_t slen;

  JString(const JString &str) :
   NativeObject<JString>(),
   JCharArray(str.length),
   slen(str.slen)
  {
   memcpy(array, str.array, slen);
  }

  JString(const JInt size) :
   NativeObject<JString>(),
   JCharArray(size),
   slen(size)
  {}

  JString(const char * const str) :
   JString((JInt)ceil(strlen(str) * (sizeof(char) / (sizeof(JChar)))))
  {
   memcpy(array, str, slen);
  }

  JInt getLength() override {
   return (JInt)slen;
  }

  bool operator==(const JString& str) {
   return strcmp((char *)array, (char *)str.array) == 0;
  }
 };
}

//TODO If we decide to link the JDK functions for java.lang.String, they would go here
DEFINE_NATIVE_TYPE(FakeJni::JString) {};