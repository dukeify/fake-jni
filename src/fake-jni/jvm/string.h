#pragma once

#include <string>

namespace FakeJni {
 class JString: public _jstring, public JCharArray, public virtual NativeObject<JString> {
 public:
  DEFINE_CLASS_NAME("java/lang/String")

//  using cast = typename _CX::ExplicitCastGenerator<JString, JCharArray, JClass, JObject, _jobject>;
  using cast = typename _CX::ExplicitCastGenerator<JString, JCharArray, JClass, JObject>;

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

//  //TODO change the ExplicitCastingGenerator to a type alias
//  //TOOD automatically generate the inverse casting route
//  inline static JString * cast(_jobject *obj) {
////   return _CX::ExplicitCastGenerator<JString, _jobject, JObject, JClass, JCharArray>::cast(obj);
//   return _CX::ExplicitCastGenerator<JString, JCharArray, JClass, JObject, _jobject>::cast(obj);
//  }
//
////  inline static JObject * cast(JString *inst) {
////   return _CX::ExplicitCastGenerator<JObject, JString, JCharArray, JClass, >;
////  }
 };
}

//TODO If we decide to link the JDK functions for
// java.lang.String, they would go here
DEFINE_NATIVE_TYPE(FakeJni::JString) {};