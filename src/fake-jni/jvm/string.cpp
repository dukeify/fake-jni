#include "fake-jni/string.h"

#include <cmath>
#include <cstring>

namespace FakeJni {
 const JString JString::EMPTY_STR {""};

 JString::JString() : JString(EMPTY_STR)
 {}

 JString::JString(const JString &str) : JString(str.getLength())
 {
  memcpy(getArray(), str.getArray(), (size_t)str.getLength());
 }

 JString::JString(const JInt size) :
  JCharArray((JInt)ceil((int)size * (sizeof(char) / ((double)sizeof(JChar))))),
  length(size)
 {}

 JString::JString(const char * str) : JString((JInt)strlen(str))
 {
  memcpy(getArray(), str, (size_t)length);
 }

 JString::JString(const JChar * str, JInt size) : JString(size * 2) {
  memcpy(getArray(), (char *)str, (size_t)size);
 }

 bool JString::operator==(const JString & str) const {
  const auto sLen = str.getLength();
  if (length == sLen) {
   return memcmp((char *)getArray(), (char *)str.getArray(), (size_t)length) == 0;
  }
  return false;
 }

 bool JString::operator==(const char * str) const {
  const auto sLen = strlen(str);
  if ((size_t)length == sLen) {
   return memcmp((char *)getArray(), str, (size_t)length) == 0;
  }
  return false;
 }

 bool JString::operator==(const std::string & str) const {
  return (*this) == str.c_str();
 }

 JString& JString::operator=(const FakeJni::JString &str) const {
  auto& ref = const_cast<JString&>(*this);
  ref.length = str.length;
  JArray<JCharArray>::operator=(str);
  return ref;
 }

 JInt JString::getLength() const {
  return length;
 }
}

DEFINE_NATIVE_DESCRIPTOR(FakeJni::JString)END_NATIVE_DESCRIPTOR