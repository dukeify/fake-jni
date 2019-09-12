#include "fake-jni/string.h"

#include <cmath>
#include <cstring>

namespace FakeJni {
 const JString JString::EMPTY_STR {""};

 JString::JString(const JString &str) : JCharArray(str.getLength())
 {
  memcpy(array, str.array, (size_t)str.getLength());
 }

 JString::JString(const JInt size) : JCharArray(size)
 {}

 JString::JString(const char * str) :
  JString((JInt)ceil((int)strlen(str) * (sizeof(char) / ((double)sizeof(JChar)))))
 {
  memcpy(array, str, (size_t)getLength());
 }

 bool JString::operator==(const JString &str) const {
  return memcmp((char *)array, (char *)str.array, (size_t)std::min(getLength(), str.getLength())) == 0;
 }
}

DEFINE_NATIVE_DESCRIPTOR(FakeJni::JString)END_NATIVE_DESCRIPTOR