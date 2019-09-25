#include "fake-jni/jvm.h"

namespace FakeJni {
 //TODO implement
 jvmtiError JvmtiInterface::getFieldName(jvmtiEnv* env, jclass klass, jfieldID field, char** name_ptr, char** signature_ptr, char** generic_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getFieldName' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getFieldDeclaringClass(jvmtiEnv* env, jclass klass, jfieldID field, jclass* declaring_class_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getFieldDeclaringClass' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getFieldModifiers(jvmtiEnv* env, jclass klass, jfieldID field, jint* modifiers_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getFieldModifiers' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::isFieldSynthetic(jvmtiEnv* env, jclass klass, jfieldID field, jboolean* is_synthetic_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::isFieldSynthetic' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setFieldAccessWatch(jvmtiEnv* env, jclass klass, jfieldID field) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setFieldAccessWatch' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::clearFieldAccessWatch(jvmtiEnv* env, jclass klass, jfieldID field) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::clearFieldAccessWatch' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setFieldModificationWatch(jvmtiEnv* env, jclass klass, jfieldID field) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setFieldModificationWatch' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::clearFieldModificationWatch(jvmtiEnv* env, jclass klass, jfieldID field) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::clearFieldModificationWatch' is unimplemented!");
 }
}