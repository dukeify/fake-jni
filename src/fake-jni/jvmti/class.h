#pragma once

namespace FakeJni {
//TODO implement
 jvmtiError JvmtiInterface::getClassSignature(jvmtiEnv* env, jclass klass, char** signature_ptr, char** generic_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getClassSignature' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getClassStatus(jvmtiEnv* env, jclass klass, jint* status_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getClassStatus' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getSourceFileName(jvmtiEnv* env, jclass klass, char** source_name_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getSourceFileName' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getClassModifiers(jvmtiEnv* env, jclass klass, jint* modifiers_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getClassModifiers' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getClassMethods(jvmtiEnv* env, jclass klass, jint* method_count_ptr, jmethodID** methods_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getClassMethods' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getClassFields(jvmtiEnv* env, jclass klass, jint* field_count_ptr, jfieldID** fields_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getClassFields' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getImplementedInterfaces(jvmtiEnv* env, jclass klass, jint* interface_count_ptr, jclass** interfaces_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getImplementedInterfaces' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::isInterface(jvmtiEnv* env, jclass klass, jboolean* is_interface_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::isInterface' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::isArrayClass(jvmtiEnv* env, jclass klass, jboolean* is_array_class_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::isArrayClass' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getClassLoader(jvmtiEnv* env, jclass klass, jobject* classloader_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getClassLoader' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::isModifiableClass(jvmtiEnv* env, jclass klass, jboolean* is_modifiable_class_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::isModifiableClass' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getLoadedClasses(jvmtiEnv* env, jint* class_count_ptr, jclass** classes_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getLoadedClasses' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getClassLoaderClasses(jvmtiEnv* env, jobject initiating_loader, jint* class_count_ptr, jclass** classes_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getClassLoaderClasses' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::redefineClasses(jvmtiEnv* env, jint class_count, const jvmtiClassDefinition* class_definitions) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::redefineClasses' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getClassVersionNumbers(jvmtiEnv* env, jclass klass, jint* minor_version_ptr, jint* major_version_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getClassVersionNumbers' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getConstantPool(jvmtiEnv* env, jclass klass, jint* constant_pool_count_ptr, jint* constant_pool_byte_count_ptr, unsigned char** constant_pool_bytes_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getConstantPool' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::addToBootstrapClassLoaderSearch(jvmtiEnv* env, const char* segment) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::addToBootstrapClassLoaderSearch' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::addToSystemClassLoaderSearch(jvmtiEnv* env, const char* segment) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::addToSystemClassLoaderSearch' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::retransformClasses(jvmtiEnv* env, jint class_count, const jclass* classes) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::retransformClasses' is unimplemented!");
 }


}