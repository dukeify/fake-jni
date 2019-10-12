#include "fake-jni/jvm.h"

#include <cx/unsafe.h>

#include <stdexcept>

namespace FakeJni {
 jfieldID NativeInterface::getFieldID(jclass jclazz, const char * name, const char * sig) const {
  auto fid = ((JClass *)jclazz)->getField(sig, name);
  if ((fid->getModifiers() & JFieldID::STATIC) == JFieldID::STATIC) {
   return nullptr;
  }
  return const_cast<JFieldID *>(fid);
 }

 jobject NativeInterface::getObjectField(jobject jobj, jfieldID jfid) const {
  return CX::union_cast<jobject>(&((JFieldID *)jfid)->get<JObject>((JObject *)jobj));
 }

 jboolean NativeInterface::getBooleanField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JBoolean>((JObject *)jobj);
 }

 jbyte NativeInterface::getByteField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JByte>((JObject *)jobj);
 }

 jchar NativeInterface::getCharField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JChar>((JObject *)jobj);
 }

 jshort NativeInterface::getShortField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JShort>((JObject *)jobj);
 }

 jint NativeInterface::getIntField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JInt>((JObject *)jobj);
 }

 jlong NativeInterface::getLongField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JLong>((JObject *)jobj);
 }

 jfloat NativeInterface::getFloatField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JFloat>((JObject *)jobj);
 }

 jdouble NativeInterface::getDoubleField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JDouble>((JObject *)jobj);
 }

 void NativeInterface::setObjectField(jobject jobj, jfieldID jfid, jobject value) const {
  ((JFieldID *)jfid)->set((JObject *)jobj, value);
 }

 void NativeInterface::setBooleanField(jobject jobj, jfieldID jfid, jboolean value) const {
  ((JFieldID *)jfid)->set((JObject *)jobj, &value);
 }

 void NativeInterface::setByteField(jobject jobj, jfieldID jfid, jbyte value) const {
  ((JFieldID *)jfid)->set((JObject *)jobj, &value);
 }

 void NativeInterface::setCharField(jobject jobj, jfieldID jfid, jchar value) const {
  ((JFieldID *)jfid)->set((JObject *)jobj, &value);
 }

 void NativeInterface::setShortField(jobject jobj, jfieldID jfid, jshort value) const {
  ((JFieldID *)jfid)->set((JObject *)jobj, &value);
 }

 void NativeInterface::setIntField(jobject jobj, jfieldID jfid, jint value) const {
  ((JFieldID *)jfid)->set((JObject *)jobj, &value);
 }

 void NativeInterface::setLongField(jobject jobj, jfieldID jfid, jlong value) const {
  ((JFieldID *)jfid)->set((JObject *)jobj, &value);
 }


 void NativeInterface::setFloatField(jobject jobj, jfieldID jfid, jfloat value) const {
  ((JFieldID *)jfid)->set((JObject *)jobj, &value);
 }

 void NativeInterface::setDoubleField(jobject jobj, jfieldID jfid, jdouble value) const {
  ((JFieldID *)jfid)->set((JObject *)jobj, &value);
 }

 jfieldID NativeInterface::getStaticFieldID(jclass jclazz, const char * name, const char * sig) const {
  auto fid = ((JClass *)jclazz)->getField(sig, name);
  if ((fid->getModifiers() & JFieldID::STATIC) == JFieldID::STATIC) {
   return const_cast<JFieldID *>(fid);
  }
  return nullptr;
 }

 jobject NativeInterface::getStaticObjectField(jclass, jfieldID jfid) const {
  return CX::union_cast<jobject>(&((JFieldID *)jfid)->get<JObject>(nullptr));
 }

 jboolean NativeInterface::getStaticBooleanField(jclass, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JBoolean>(nullptr);
 }

 jbyte NativeInterface::getStaticByteField(jclass, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JBoolean>(nullptr);
 }

 jchar NativeInterface::getStaticCharField(jclass, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JChar>(nullptr);
 }

 jshort NativeInterface::getStaticShortField(jclass, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JShort>(nullptr);
 }

 jint NativeInterface::getStaticIntField(jclass, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JInt>(nullptr);
 }

 jlong NativeInterface::getStaticLongField(jclass, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStaticLongField' is unimplemented!");
  return 0;
 }

 jfloat NativeInterface::getStaticFloatField(jclass, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JFloat>(nullptr);
 }

 jdouble NativeInterface::getStaticDoubleField(jclass, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get<JDouble>(nullptr);
 }

 void NativeInterface::setStaticObjectField(jclass, jfieldID jfid, jobject value) const {
  ((JFieldID *)jfid)->set(nullptr, value);
 }

 void NativeInterface::setStaticBooleanField(jclass, jfieldID jfid, jboolean value) const {
  ((JFieldID *)jfid)->set(nullptr, &value);
 }

 void NativeInterface::setStaticByteField(jclass, jfieldID jfid, jbyte value) const {
  ((JFieldID *)jfid)->set(nullptr, &value);
 }

 void NativeInterface::setStaticCharField(jclass, jfieldID jfid, jchar value) const {
  ((JFieldID *)jfid)->set(nullptr, &value);
 }

 void NativeInterface::setStaticShortField(jclass, jfieldID jfid, jshort value) const {
  ((JFieldID *)jfid)->set(nullptr, &value);
 }

 void NativeInterface::setStaticIntField(jclass, jfieldID jfid, jint value) const {
  ((JFieldID *)jfid)->set(nullptr, &value);
 }

 void NativeInterface::setStaticLongField(jclass, jfieldID jfid, jlong value) const {
  ((JFieldID *)jfid)->set(nullptr, &value);
 }

 void NativeInterface::setStaticFloatField(jclass, jfieldID jfid, jfloat value) const {
  ((JFieldID *)jfid)->set(nullptr, &value);
 }

 void NativeInterface::setStaticDoubleField(jclass, jfieldID jfid, jdouble value) const {
  ((JFieldID *)jfid)->set(nullptr, &value);
 }
}