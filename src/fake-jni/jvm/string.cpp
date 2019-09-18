#include "fake-jni/string.h"

#include <cmath>
#include <cstring>

namespace FakeJni {
 const JString JString::EMPTY_STR {""};

 JString::JString() : JString(EMPTY_STR)
 {}

 JString::JString(const JString &str) : JString(str.getLength())
 {
  memcpy(array, str.array, (size_t)str.getLength());
 }

 JString::JString(const JInt size) :
  JCharArray((JInt)ceil((int)size * (sizeof(char) / ((double)sizeof(JChar))))),
  length(size)
 {}

 JString::JString(const char * str) : JString((JInt)strlen(str))
 {
  memcpy(array, str, (size_t)length);
 }

 bool JString::operator==(const JString & str) const {
  return memcmp((char *)array, (char *)str.array, (size_t)std::min(getLength(), str.getLength())) == 0;
 }

 bool JString::operator==(const char * str) const {
  return memcmp((char *)array, str, std::min((size_t)getLength(), strlen(str))) == 0;
 }

 bool JString::operator==(const std::string & str) const {
  return (*this) == str.c_str();
 }

 JString& JString::operator=(const FakeJni::JString &str) const {
  return const_cast<JString&>(*this);
 }

 JInt JString::getLength() const {
  return length;
 }
}

DEFINE_NATIVE_DESCRIPTOR(FakeJni::JString)END_NATIVE_DESCRIPTOR