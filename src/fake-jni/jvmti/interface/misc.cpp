#include "jni.h"
#include "jvmti.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
//TODO implement
 jvmtiError JvmtiInterface::setEventNotificationMode(jvmtiEnv* env, jvmtiEventMode mode, jvmtiEvent event_type, jthread event_thread) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setEventNotificationMode' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::allocate(jvmtiEnv* env, jlong size, unsigned char** mem_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::allocate' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::deallocate(jvmtiEnv* env, unsigned char* mem) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::deallocate' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setBreakpoint(jvmtiEnv* env, jmethodID method, jlocation location) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setBreakpoint' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::clearBreakpoint(jvmtiEnv* env, jmethodID method, jlocation location) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::clearBreakpoint' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getObjectHashCode(jvmtiEnv* env, jobject object, jint* hash_code_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getObjectHashCode' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::popFrame(jvmtiEnv* env, jthread thread) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::popFrame' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::forceEarlyReturnObject(jvmtiEnv* env, jthread thread, jobject value) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::forceEarlyReturnObject' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::forceEarlyReturnInt(jvmtiEnv* env, jthread thread, jint value) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::forceEarlyReturnInt' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::forceEarlyReturnLong(jvmtiEnv* env, jthread thread, jlong value) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::forceEarlyReturnLong' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::forceEarlyReturnFloat(jvmtiEnv* env, jthread thread, jfloat value) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::forceEarlyReturnFloat' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::forceEarlyReturnDouble(jvmtiEnv* env, jthread thread, jdouble value) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::forceEarlyReturnDouble' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::forceEarlyReturnVoid(jvmtiEnv* env, jthread thread) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::forceEarlyReturnVoid' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getVersionNumber(jvmtiEnv* env, jint* version_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getVersionNumber' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getCapabilities(jvmtiEnv* env, jvmtiCapabilities* capabilities_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getCapabilities' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getSourceDebugExtension(jvmtiEnv* env, jclass klass, char** source_debug_extension_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getSourceDebugExtension' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getTag(jvmtiEnv* env, jobject object, jlong* tag_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getTag' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setTag(jvmtiEnv* env, jobject object, jlong tag) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setTag' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::forceGarbageCollection(jvmtiEnv* env) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::forceGarbageCollection' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::iterateOverObjectsReachableFromObject(jvmtiEnv* env, jobject object, jvmtiObjectReferenceCallback object_reference_callback, const void* user_data) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::iterateOverObjectsReachableFromObject' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::iterateOverReachableObjects(jvmtiEnv* env, jvmtiHeapRootCallback heap_root_callback, jvmtiStackReferenceCallback stack_ref_callback, jvmtiObjectReferenceCallback object_ref_callback, const void* user_data) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::iterateOverReachableObjects' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::iterateOverHeap(jvmtiEnv* env, jvmtiHeapObjectFilter object_filter, jvmtiHeapObjectCallback heap_object_callback, const void* user_data) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::iterateOverHeap' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::iterateOverInstancesOfClass(jvmtiEnv* env, jclass klass, jvmtiHeapObjectFilter object_filter, jvmtiHeapObjectCallback heap_object_callback, const void* user_data) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::iterateOverInstancesOfClass' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getObjectsWithTags(jvmtiEnv* env, jint tag_count, const jlong* tags, jint* count_ptr, jobject** object_result_ptr, jlong** tag_result_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getObjectsWithTags' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::followReferences(jvmtiEnv* env, jint heap_filter, jclass klass, jobject initial_object, const jvmtiHeapCallbacks* callbacks, const void* user_data) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::followReferences' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::iterateThroughHeap(jvmtiEnv* env, jint heap_filter, jclass klass, const jvmtiHeapCallbacks* callbacks, const void* user_data) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::iterateThroughHeap' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setJNIFunctionTable(jvmtiEnv* env, const jniNativeInterface* function_table) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setJNIFunctionTable' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getJNIFunctionTable(jvmtiEnv* env, jniNativeInterface** function_table) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getJNIFunctionTable' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setEventCallbacks(jvmtiEnv* env, const jvmtiEventCallbacks* callbacks, jint size_of_callbacks) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setEventCallbacks' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::generateEvents(jvmtiEnv* env, jvmtiEvent event_type) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::generateEvents' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getExtensionFunctions(jvmtiEnv* env, jint* extension_count_ptr, jvmtiExtensionFunctionInfo** extensions) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getExtensionFunctions' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getExtensionEvents(jvmtiEnv* env, jint* extension_count_ptr, jvmtiExtensionEventInfo** extensions) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getExtensionEvents' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setExtensionEventCallback(jvmtiEnv* env, jint extension_event_index, jvmtiExtensionEvent callback) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setExtensionEventCallback' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::disposeEnvironment(jvmtiEnv* env) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::disposeEnvironment' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getErrorName(jvmtiEnv* env, jvmtiError error, char** name_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getErrorName' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getJLocationFormat(jvmtiEnv* env,jvmtiJlocationFormat* format_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getJLocationFormat' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getSystemProperties(jvmtiEnv* env, jint* count_ptr, char*** property_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getSystemProperties' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getSystemProperty(jvmtiEnv* env, const char* property, char** value_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getSystemProperty' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setSystemProperty(jvmtiEnv* env, const char* property, const char* value) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setSystemProperty' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getPhase(jvmtiEnv* env, jvmtiPhase* phase_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getPhase' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getTimerInfo(jvmtiEnv* env, jvmtiTimerInfo* info_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getTimerInfo' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getTime(jvmtiEnv* env, jlong* nanos_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getTime' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getPotentialCapabilities(jvmtiEnv* env, jvmtiCapabilities* capabilities_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getPotentialCapabilities' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::addCapabilities(jvmtiEnv* env, const jvmtiCapabilities* capabilities_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::addCapabilities' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::relinquishCapabilities(jvmtiEnv* env, const jvmtiCapabilities* capabilities_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::relinquishCapabilities' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getAvailableProcessors(jvmtiEnv* env, jint* processor_count_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getAvailableProcessors' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getEnvironmentLocalStorage(jvmtiEnv* env, void** data_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getEnvironmentLocalStorage' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setEnvironmentLocalStorage(jvmtiEnv* env, const void* data) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setEnvironmentLocalStorage' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setVerboseFlag(jvmtiEnv* env, jvmtiVerboseFlag flag, jboolean value) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setVerboseFlag' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getObjectSize(jvmtiEnv* env, jobject object, jlong* size_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getObjectSize' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getLocalInstance(jvmtiEnv* env, jthread thread, jint depth, jobject* value_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getLocalInstance' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getFrameLocation(jvmtiEnv* env, jthread thread, jint depth, jmethodID* method_ptr, jlocation* location_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getFrameLocation' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::notifyFramePop(jvmtiEnv* env, jthread thread, jint depth) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::notifyFramePop' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getLocalObject(jvmtiEnv* env, jthread thread, jint depth, jint slot, jobject* value_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getLocalObject' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getLocalInt(jvmtiEnv* env, jthread thread, jint depth, jint slot, jint* value_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getLocalInt' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getLocalLong(jvmtiEnv* env, jthread thread, jint depth, jint slot, jlong* value_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getLocalLong' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getLocalFloat(jvmtiEnv* env, jthread thread, jint depth, jint slot, jfloat* value_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getLocalFloat' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getLocalDouble(jvmtiEnv* env, jthread thread, jint depth, jint slot, jdouble* value_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getLocalDouble' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setLocalObject(jvmtiEnv* env, jthread thread, jint depth, jint slot, jobject value) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setLocalObject' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setLocalInt(jvmtiEnv* env, jthread thread, jint depth, jint slot, jint value) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setLocalInt' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setLocalLong(jvmtiEnv* env, jthread thread, jint depth, jint slot, jlong value) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setLocalLong' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setLocalFloat(jvmtiEnv* env, jthread thread, jint depth, jint slot, jfloat value) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setLocalFloat' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setLocalDouble(jvmtiEnv* env, jthread thread, jint depth, jint slot, jdouble value) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setLocalDouble' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getAllStackTraces(jvmtiEnv* env, jint max_frame_count, jvmtiStackInfo** stack_info_ptr, jint* thread_count_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getAllStackTraces' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getStackTrace(jvmtiEnv* env, jthread thread, jint start_depth, jint max_frame_count, jvmtiFrameInfo* frame_buffer, jint* count_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getStackTrace' is unimplemented!");
 }
}