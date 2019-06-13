#pragma once

namespace FakeJni {
 class JString: public _jstring, public JCharArray {
 public:
  DEFINE_CLASS_NAME("java/lang/String")

  JString(const JString &str): JCharArray(str.length) {
   memcpy(array, str.array, length);
  }

  JString(const JInt size): JCharArray(size) {}

  JString(const char * const str): JString(strlen(str)) {
   memcpy(array, str, length);
  }

  inline static JString * cast(_jobject *obj) {
   return _CX::DowncastingGenerator<JString, _jobject, JObject, JClass>::cast(obj);
  }
 };
}

//TODO If we decide to link the JDK functions for
// java.lang.String, they would go here
DEFINE_NATIVE_TYPE(FakeJni::JString) {};