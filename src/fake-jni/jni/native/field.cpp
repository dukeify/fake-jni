#include "fake-jni/jvm.h"

namespace FakeJni {
 jfieldID NativeInterface::getFieldID(jclass jclazz, const char * name, const char * sig) const {
  auto fid = ((JClass *)*jclazz)->getField(sig, name);
  if (fid) {
   if ((fid->getModifiers() & JFieldID::STATIC) != JFieldID::STATIC) {
    return const_cast<JFieldID *>(fid);
   }
  }
  return nullptr;
 }

 jobject NativeInterface::getObjectField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get(*jobj);
 }

 jboolean NativeInterface::getBooleanField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get(*jobj);
 }

 jbyte NativeInterface::getByteField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get(*jobj);
 }

 jchar NativeInterface::getCharField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get(*jobj);
 }

 jshort NativeInterface::getShortField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get(*jobj);
 }

 jint NativeInterface::getIntField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get(*jobj);
 }

 jlong NativeInterface::getLongField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get(*jobj);
 }

 jfloat NativeInterface::getFloatField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get(*jobj);
 }

 jdouble NativeInterface::getDoubleField(jobject jobj, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get(*jobj);
 }

 void NativeInterface::setObjectField(jobject jobj, jfieldID jfid, jobject value) const {
  ((JFieldID *)jfid)->set(*jobj, value);
 }

 void NativeInterface::setBooleanField(jobject jobj, jfieldID jfid, jboolean value) const {
  ((JFieldID *)jfid)->set(*jobj, &value);
 }

 void NativeInterface::setByteField(jobject jobj, jfieldID jfid, jbyte value) const {
  ((JFieldID *)jfid)->set(*jobj, &value);
 }

 void NativeInterface::setCharField(jobject jobj, jfieldID jfid, jchar value) const {
  ((JFieldID *)jfid)->set(*jobj, &value);
 }

 void NativeInterface::setShortField(jobject jobj, jfieldID jfid, jshort value) const {
  ((JFieldID *)jfid)->set(*jobj, &value);
 }

 void NativeInterface::setIntField(jobject jobj, jfieldID jfid, jint value) const {
  ((JFieldID *)jfid)->set(*jobj, &value);
 }

 void NativeInterface::setLongField(jobject jobj, jfieldID jfid, jlong value) const {
  ((JFieldID *)jfid)->set(*jobj, &value);
 }


 void NativeInterface::setFloatField(jobject jobj, jfieldID jfid, jfloat value) const {
  ((JFieldID *)jfid)->set(*jobj, &value);
 }

 void NativeInterface::setDoubleField(jobject jobj, jfieldID jfid, jdouble value) const {
  ((JFieldID *)jfid)->set(*jobj, &value);
 }

 jfieldID NativeInterface::getStaticFieldID(jclass jclazz, const char * name, const char * sig) const {
  auto fid = ((JClass *)*jclazz)->getField(sig, name);
  if (fid) {
   if ((fid->getModifiers() & JFieldID::STATIC) == JFieldID::STATIC) {
    return const_cast<JFieldID *>(fid);
   }
  }
  return nullptr;
 }

 jobject NativeInterface::getStaticObjectField(jclass jclazz, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get((JClass *)*jclazz);
 }

 jboolean NativeInterface::getStaticBooleanField(jclass jclazz, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get((JClass *)*jclazz);
 }

 jbyte NativeInterface::getStaticByteField(jclass jclazz, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get((JClass *)*jclazz);
 }

 jchar NativeInterface::getStaticCharField(jclass jclazz, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get((JClass *)*jclazz);
 }

 jshort NativeInterface::getStaticShortField(jclass jclazz, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get((JClass *)*jclazz);
 }

 jint NativeInterface::getStaticIntField(jclass jclazz, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get((JClass *)*jclazz);
 }

 jlong NativeInterface::getStaticLongField(jclass jclazz, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get((JClass *)*jclazz);
 }

 jfloat NativeInterface::getStaticFloatField(jclass jclazz, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get((JClass *)*jclazz);
 }

 jdouble NativeInterface::getStaticDoubleField(jclass jclazz, jfieldID jfid) const {
  return ((JFieldID *)jfid)->get((JClass *)*jclazz);
 }

 void NativeInterface::setStaticObjectField(jclass jclazz, jfieldID jfid, jobject value) const {
  ((JFieldID *)jfid)->set((JClass *)*jclazz, value);
 }

 void NativeInterface::setStaticBooleanField(jclass jclazz, jfieldID jfid, jboolean value) const {
  ((JFieldID *)jfid)->set((JClass *)*jclazz, &value);
 }

 void NativeInterface::setStaticByteField(jclass jclazz, jfieldID jfid, jbyte value) const {
  ((JFieldID *)jfid)->set((JClass *)*jclazz, &value);
 }

 void NativeInterface::setStaticCharField(jclass jclazz, jfieldID jfid, jchar value) const {
  ((JFieldID *)jfid)->set((JClass *)*jclazz, &value);
 }

 void NativeInterface::setStaticShortField(jclass jclazz, jfieldID jfid, jshort value) const {
  ((JFieldID *)jfid)->set((JClass *)*jclazz, &value);
 }

 void NativeInterface::setStaticIntField(jclass jclazz, jfieldID jfid, jint value) const {
  ((JFieldID *)jfid)->set((JClass *)*jclazz, &value);
 }

 void NativeInterface::setStaticLongField(jclass jclazz, jfieldID jfid, jlong value) const {
  ((JFieldID *)jfid)->set((JClass *)*jclazz, &value);
 }

 void NativeInterface::setStaticFloatField(jclass jclazz, jfieldID jfid, jfloat value) const {
  ((JFieldID *)jfid)->set((JClass *)*jclazz, &value);
 }

 void NativeInterface::setStaticDoubleField(jclass jclazz, jfieldID jfid, jdouble value) const {
  ((JFieldID *)jfid)->set((JClass *)*jclazz, &value);
 }
}