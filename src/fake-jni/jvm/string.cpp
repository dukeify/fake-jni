#include "fake-jni/jvm.h"

#include <cmath>
#include <cstring>

namespace FakeJni {
 const JString JString::EMPTY_STR {""};

 JString::JString(const JString &str) :
  NativeObject<JString>(),
  JCharArray(str.length),
  slen(str.slen)
 {
  memcpy(array, str.array, slen);
 }

 JString::JString(const JInt size) :
  NativeObject<JString>(),
  JCharArray(size),
  slen(size)
 {}

 JString::JString(const char * const str) :
  JString((JInt)ceil(strlen(str) * (sizeof(char) / ((float)sizeof(JChar)))))
 {
  memcpy(array, str, slen);
 }

 bool JString::operator==(const JString& str) const {
  return strcmp((char *)array, (char *)str.array) == 0;
 }
}

//TODO If we decide to link the JDK functions for java.lang.String, they would go here
DEFINE_NATIVE_DESCRIPTOR(FakeJni::JString) {};