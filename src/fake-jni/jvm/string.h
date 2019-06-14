#pragma once

#include <string>

namespace FakeJni {
 class JString: public _jstring, public JCharArray, public virtual NativeObject<JString> {
 public:
  DEFINE_CLASS_NAME("java/lang/String")

  JString(const JString &str):
   NativeObject<JString>(),
   JCharArray(str.length)
  {
   memcpy(array, str.array, length);
  }

  JString(const JInt size):
   NativeObject<JString>(),
   JCharArray(size)
  {}

  JString(const char * const str): JString((strlen(str) * (sizeof(char) / (sizeof(JChar)))) + 1) {
   memcpy(array, str, strlen(str));
  }

  bool operator==(const JString& str) {
   return strcmp((char *)array, (char *)str.array) == 0;
  }

  inline static JString * cast(_jobject *obj) {
   return _CX::DowncastingGenerator<JString, _jobject, JObject, JClass, JCharArray>::cast(obj);
  }
 };
}

//TODO If we decide to link the JDK functions for
// java.lang.String, they would go here
DEFINE_NATIVE_TYPE(FakeJni::JString) {};