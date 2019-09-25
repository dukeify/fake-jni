#include "fake-jni/jvm.h"

namespace FakeJni {
//TODO implement
 jvmtiError JvmtiInterface::getMethodName(jvmtiEnv* env, jmethodID method, char** name_ptr, char** signature_ptr, char** generic_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getMethodName' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getMethodDeclaringClass(jvmtiEnv* env, jmethodID method, jclass* declaring_class_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getMethodDeclaringClass' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getMethodModifiers(jvmtiEnv* env, jmethodID method, jint* modifiers_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getMethodModifiers' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getMaxLocals(jvmtiEnv* env, jmethodID method, jint* max_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getMaxLocals' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getArgumentsSize(jvmtiEnv* env, jmethodID method, jint* size_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getArgumentsSize' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getLineNumberTable(jvmtiEnv* env, jmethodID method, jint* entry_count_ptr, jvmtiLineNumberEntry** table_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getLineNumberTable' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getMethodLocation(jvmtiEnv* env, jmethodID method, jlocation* start_location_ptr, jlocation* end_location_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getMethodLocation' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getLocalVariableTable(jvmtiEnv* env, jmethodID method, jint* entry_count_ptr, jvmtiLocalVariableEntry** table_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getLocalVariableTable' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setNativeMethodPrefix(jvmtiEnv* env, const char* prefix) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setNativeMethodPrefix' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setNativeMethodPrefixes(jvmtiEnv* env, jint prefix_count, char** prefixes) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setNativeMethodPrefixes' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getBytecodes(jvmtiEnv* env, jmethodID method, jint* bytecode_count_ptr, unsigned char** bytecodes_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getBytecodes' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::isMethodNative(jvmtiEnv* env, jmethodID method, jboolean* is_native_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::isMethodNative' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::isMethodSynthetic(jvmtiEnv* env, jmethodID method, jboolean* is_synthetic_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::isMethodSynthetic' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::isMethodObsolete(jvmtiEnv* env, jmethodID method, jboolean* is_obsolete_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::isMethodObsolete' is unimplemented!");
 }
}