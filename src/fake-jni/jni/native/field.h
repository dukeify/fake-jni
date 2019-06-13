#pragma once

namespace FakeJni {
//TODO implement
 jfieldID NativeInterface::getFieldID(jclass, const char *, const char *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getFieldID' is unimplemented!");
  return 0;
 }

//TODO implement
 jobject NativeInterface::getObjectField(jobject, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getObjectField' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jboolean NativeInterface::getBooleanField(jobject, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getBooleanField' is unimplemented!");
  return 0;
 }

//TODO implement
 jbyte NativeInterface::getByteField(jobject, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getByteField' is unimplemented!");
  return 0;
 }

//TODO implement
 jchar NativeInterface::getCharField(jobject, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getCharField' is unimplemented!");
  return 0;
 }

//TODO implement
 jshort NativeInterface::getShortField(jobject, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getShortField' is unimplemented!");
  return 0;
 }

//TODO implement
 jint NativeInterface::getIntField(jobject, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getIntField' is unimplemented!");
  return 0;
 }

//TODO implement
 jlong NativeInterface::getLongField(jobject, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getLongField' is unimplemented!");
  return 0;
 }

//TODO implement
 jfloat NativeInterface::getFloatField(jobject, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getFloatField' is unimplemented!");
  return 0.0f;
 }

//TODO implement
 jdouble NativeInterface::getDoubleField(jobject, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getDoubleField' is unimplemented!");
  return 0.0;
 }

//TODO implement
 void NativeInterface::setObjectField(jobject, jfieldID, jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setObjectField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setBooleanField(jobject, jfieldID, jboolean b) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setBooleanField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setByteField(jobject, jfieldID, jbyte) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setByteField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setCharField(jobject, jfieldID, jchar) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setCharField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setShortField(jobject, jfieldID, jshort) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setShortField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setIntField(jobject, jfieldID, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setIntField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setLongField(jobject, jfieldID, jlong) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setLongField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setFloatField(jobject, jfieldID, jfloat) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setFloatField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setDoubleField(jobject, jfieldID, jdouble) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setDoubleField' is unimplemented!");
 }

//TODO implement
 jfieldID NativeInterface::getStaticFieldID(jclass, const char *, const char *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStaticFieldID' is unimplemented!");
  return 0;
 }

//TODO implement
 jobject NativeInterface::getStaticObjectField(jclass, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStaticObjectField' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jboolean NativeInterface::getStaticBooleanField(jclass, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStaticBooleanField' is unimplemented!");
  return 0;
 }

//TODO implement
 jbyte NativeInterface::getStaticByteField(jclass, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStaticByteField' is unimplemented!");
  return 0;
 }

//TODO implement
 jchar NativeInterface::getStaticCharField(jclass, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStaticCharField' is unimplemented!");
  return 0;
 }

//TODO implement
 jshort NativeInterface::getStaticShortField(jclass, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStaticShortField' is unimplemented!");
  return 0;
 }

//TODO implement
 jint NativeInterface::getStaticIntField(jclass, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStaticIntField' is unimplemented!");
  return 0;
 }

//TODO implement
 jlong NativeInterface::getStaticLongField(jclass, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStaticLongField' is unimplemented!");
  return 0;
 }

//TODO implement
 jfloat NativeInterface::getStaticFloatField(jclass, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStaticFloatField' is unimplemented!");
  return 0.0f;
 }

//TODO implement
 jdouble NativeInterface::getStaticDoubleField(jclass, jfieldID) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getStaticDoubleField' is unimplemented!");
  return 0.0;
 }

//TODO implement
 void NativeInterface::setStaticObjectField(jclass, jfieldID, jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setStaticObjectField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setStaticBooleanField(jclass, jfieldID, jboolean b) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setStaticBooleanField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setStaticByteField(jclass, jfieldID, jbyte) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setStaticByteField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setStaticCharField(jclass, jfieldID, jchar) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setStaticCharField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setStaticShortField(jclass, jfieldID, jshort) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setStaticShortField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setStaticIntField(jclass, jfieldID, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setStaticIntField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setStaticLongField(jclass, jfieldID, jlong) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setStaticLongField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setStaticFloatField(jclass, jfieldID, jfloat) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setStaticFloatField' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setStaticDoubleField(jclass, jfieldID, jdouble) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setStaticDoubleField' is unimplemented!");
 }
}