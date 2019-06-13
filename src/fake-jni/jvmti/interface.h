#pragma once

namespace FakeJni {
 class JvmtiInterface: public jvmtiInterface_1 {
 private:
  //interface_inlined.h
  inline static jvmtiError _SetEventNotificationMode(jvmtiEnv* env, jvmtiEventMode mode, jvmtiEvent event_type, jthread event_thread, ...);

 public:
  //interface_constructor.h
  JvmtiInterface();

  //thread.h
  virtual jvmtiError getAllThreads(jvmtiEnv* env, jint* threads_count_ptr, jthread** threads_ptr) const;
  virtual jvmtiError suspendThread(jvmtiEnv* env, jthread thread) const;
  virtual jvmtiError resumeThread(jvmtiEnv* env, jthread thread) const;
  virtual jvmtiError stopThread(jvmtiEnv* env, jthread thread, jobject exception) const;
  virtual jvmtiError interruptThread(jvmtiEnv* env, jthread thread) const;
  virtual jvmtiError getThreadInfo(jvmtiEnv* env, jthread thread, jvmtiThreadInfo* info_ptr) const;
  virtual jvmtiError runAgentThread(jvmtiEnv* env, jthread thread, jvmtiStartFunction proc, const void* arg, jint priority) const;
  virtual jvmtiError getTopThreadGroups(jvmtiEnv* env, jint* group_count_ptr, jthreadGroup** groups_ptr) const;
  virtual jvmtiError getThreadGroupInfo(jvmtiEnv* env, jthreadGroup group, jvmtiThreadGroupInfo* info_ptr) const;
  virtual jvmtiError getThreadGroupChildren(jvmtiEnv* env, jthreadGroup group, jint* thread_count_ptr, jthread** threads_ptr, jint* group_count_ptr, jthreadGroup** groups_ptr) const;
  virtual jvmtiError getFrameCount(jvmtiEnv* env, jthread thread, jint* count_ptr) const;
  virtual jvmtiError getThreadState(jvmtiEnv* env, jthread thread, jint* thread_state_ptr) const;
  virtual jvmtiError getCurrentThread(jvmtiEnv* env, jthread* thread_ptr) const;
  virtual jvmtiError suspendThreadList(jvmtiEnv* env, jint request_count, const jthread* request_list, jvmtiError* results) const;
  virtual jvmtiError resumeThreadList(jvmtiEnv* env, jint request_count, const jthread* request_list, jvmtiError* results) const;
  virtual jvmtiError getThreadListStackTraces(jvmtiEnv* env, jint thread_count, const jthread* thread_list, jint max_frame_count, jvmtiStackInfo** stack_info_ptr) const;
  virtual jvmtiError getThreadLocalStorage(jvmtiEnv* env, jthread thread, void** data_ptr) const;
  virtual jvmtiError setThreadLocalStorage(jvmtiEnv* env, jthread thread, const void* data) const;
  virtual jvmtiError getCurrentThreadCpuTimerInfo(jvmtiEnv* env, jvmtiTimerInfo* info_ptr) const;
  virtual jvmtiError getCurrentThreadCpuTime(jvmtiEnv* env, jlong* nanos_ptr) const;
  virtual jvmtiError getThreadCpuTimerInfo(jvmtiEnv* env, jvmtiTimerInfo* info_ptr) const;
  virtual jvmtiError getThreadCpuTime(jvmtiEnv* env, jthread thread, jlong* nanos_ptr) const;

  //monitor.h
  virtual jvmtiError getOwnedMonitorInfo(jvmtiEnv* env, jthread thread, jint* owned_monitor_count_ptr, jobject** owned_monitors_ptr) const;
  virtual jvmtiError getCurrentContendedMonitor(jvmtiEnv* env, jthread thread, jobject* monitor_ptr) const;
  virtual jvmtiError createRawMonitor(jvmtiEnv* env, const char* name, jrawMonitorID* monitor_ptr) const;
  virtual jvmtiError destroyRawMonitor(jvmtiEnv* env, jrawMonitorID monitor) const;
  virtual jvmtiError rawMonitorEnter(jvmtiEnv* env, jrawMonitorID monitor) const;
  virtual jvmtiError rawMonitorExit(jvmtiEnv* env, jrawMonitorID monitor) const;
  virtual jvmtiError rawMonitorWait(jvmtiEnv* env, jrawMonitorID monitor, jlong millis) const;
  virtual jvmtiError rawMonitorNotify(jvmtiEnv* env, jrawMonitorID monitor) const;
  virtual jvmtiError rawMonitorNotifyAll(jvmtiEnv* env, jrawMonitorID monitor) const;
  virtual jvmtiError getObjectMonitorUsage(jvmtiEnv* env, jobject object, jvmtiMonitorUsage* info_ptr) const;
  virtual jvmtiError getOwnedMonitorStackDepthInfo(jvmtiEnv* env, jthread thread, jint* monitor_info_count_ptr, jvmtiMonitorStackDepthInfo** monitor_info_ptr) const;

  //class.h
  virtual jvmtiError getClassSignature(jvmtiEnv* env, jclass klass, char** signature_ptr, char** generic_ptr) const;
  virtual jvmtiError getClassStatus(jvmtiEnv* env, jclass klass, jint* status_ptr) const;
  virtual jvmtiError getSourceFileName(jvmtiEnv* env, jclass klass, char** source_name_ptr) const;
  virtual jvmtiError getClassModifiers(jvmtiEnv* env, jclass klass, jint* modifiers_ptr) const;
  virtual jvmtiError getClassMethods(jvmtiEnv* env, jclass klass, jint* method_count_ptr, jmethodID** methods_ptr) const;
  virtual jvmtiError getClassFields(jvmtiEnv* env, jclass klass, jint* field_count_ptr, jfieldID** fields_ptr) const;
  virtual jvmtiError getImplementedInterfaces(jvmtiEnv* env, jclass klass, jint* interface_count_ptr, jclass** interfaces_ptr) const;
  virtual jvmtiError isInterface(jvmtiEnv* env, jclass klass, jboolean* is_interface_ptr) const;
  virtual jvmtiError isArrayClass(jvmtiEnv* env, jclass klass, jboolean* is_array_class_ptr) const;
  virtual jvmtiError getClassLoader(jvmtiEnv* env, jclass klass, jobject* classloader_ptr) const;
  virtual jvmtiError isModifiableClass(jvmtiEnv* env, jclass klass, jboolean* is_modifiable_class_ptr) const;
  virtual jvmtiError getLoadedClasses(jvmtiEnv* env, jint* class_count_ptr, jclass** classes_ptr) const;
  virtual jvmtiError getClassLoaderClasses(jvmtiEnv* env, jobject initiating_loader, jint* class_count_ptr, jclass** classes_ptr) const;
  virtual jvmtiError redefineClasses(jvmtiEnv* env, jint class_count, const jvmtiClassDefinition* class_definitions) const;
  virtual jvmtiError getClassVersionNumbers(jvmtiEnv* env, jclass klass, jint* minor_version_ptr, jint* major_version_ptr) const;
  virtual jvmtiError getConstantPool(jvmtiEnv* env, jclass klass, jint* constant_pool_count_ptr, jint* constant_pool_byte_count_ptr, unsigned char** constant_pool_bytes_ptr) const;
  virtual jvmtiError addToBootstrapClassLoaderSearch(jvmtiEnv* env, const char* segment) const;
  virtual jvmtiError addToSystemClassLoaderSearch(jvmtiEnv* env, const char* segment) const;
  virtual jvmtiError retransformClasses(jvmtiEnv* env, jint class_count, const jclass* classes) const;

  //field.h
  virtual jvmtiError getFieldName(jvmtiEnv* env, jclass klass, jfieldID field, char** name_ptr, char** signature_ptr, char** generic_ptr) const;
  virtual jvmtiError getFieldDeclaringClass(jvmtiEnv* env, jclass klass, jfieldID field, jclass* declaring_class_ptr) const;
  virtual jvmtiError getFieldModifiers(jvmtiEnv* env, jclass klass, jfieldID field, jint* modifiers_ptr) const;
  virtual jvmtiError isFieldSynthetic(jvmtiEnv* env, jclass klass, jfieldID field, jboolean* is_synthetic_ptr) const;
  virtual jvmtiError setFieldAccessWatch(jvmtiEnv* env, jclass klass, jfieldID field) const;
  virtual jvmtiError clearFieldAccessWatch(jvmtiEnv* env, jclass klass, jfieldID field) const;
  virtual jvmtiError setFieldModificationWatch(jvmtiEnv* env, jclass klass, jfieldID field) const;
  virtual jvmtiError clearFieldModificationWatch(jvmtiEnv* env, jclass klass, jfieldID field) const;

  //method.h
  virtual jvmtiError getMethodName(jvmtiEnv* env, jmethodID method, char** name_ptr, char** signature_ptr, char** generic_ptr) const;
  virtual jvmtiError getMethodDeclaringClass(jvmtiEnv* env, jmethodID method, jclass* declaring_class_ptr) const;
  virtual jvmtiError getMethodModifiers(jvmtiEnv* env, jmethodID method, jint* modifiers_ptr) const;
  virtual jvmtiError getMaxLocals(jvmtiEnv* env, jmethodID method, jint* max_ptr) const;
  virtual jvmtiError getArgumentsSize(jvmtiEnv* env, jmethodID method, jint* size_ptr) const;
  virtual jvmtiError getLineNumberTable(jvmtiEnv* env, jmethodID method, jint* entry_count_ptr, jvmtiLineNumberEntry** table_ptr) const;
  virtual jvmtiError getMethodLocation(jvmtiEnv* env, jmethodID method, jlocation* start_location_ptr, jlocation* end_location_ptr) const;
  virtual jvmtiError getLocalVariableTable(jvmtiEnv* env, jmethodID method, jint* entry_count_ptr, jvmtiLocalVariableEntry** table_ptr) const;
  virtual jvmtiError setNativeMethodPrefix(jvmtiEnv* env, const char* prefix) const;
  virtual jvmtiError setNativeMethodPrefixes(jvmtiEnv* env, jint prefix_count, char** prefixes) const;
  virtual jvmtiError getBytecodes(jvmtiEnv* env, jmethodID method, jint* bytecode_count_ptr, unsigned char** bytecodes_ptr) const;
  virtual jvmtiError isMethodNative(jvmtiEnv* env, jmethodID method, jboolean* is_native_ptr) const;
  virtual jvmtiError isMethodSynthetic(jvmtiEnv* env, jmethodID method, jboolean* is_synthetic_ptr) const;
  virtual jvmtiError isMethodObsolete(jvmtiEnv* env, jmethodID method, jboolean* is_obsolete_ptr) const;

  //misc.h
  virtual jvmtiError setEventNotificationMode(jvmtiEnv* env, jvmtiEventMode mode, jvmtiEvent event_type, jthread event_thread) const;
  virtual jvmtiError allocate(jvmtiEnv* env, jlong size, unsigned char** mem_ptr) const;
  virtual jvmtiError deallocate(jvmtiEnv* env, unsigned char* mem) const;
  virtual jvmtiError setBreakpoint(jvmtiEnv* env, jmethodID method, jlocation location) const;
  virtual jvmtiError clearBreakpoint(jvmtiEnv* env, jmethodID method, jlocation location) const;
  virtual jvmtiError getObjectHashCode(jvmtiEnv* env, jobject object, jint* hash_code_ptr) const;
  virtual jvmtiError popFrame(jvmtiEnv* env, jthread thread) const;
  virtual jvmtiError forceEarlyReturnObject(jvmtiEnv* env, jthread thread, jobject value) const;
  virtual jvmtiError forceEarlyReturnInt(jvmtiEnv* env, jthread thread, jint value) const;
  virtual jvmtiError forceEarlyReturnLong(jvmtiEnv* env, jthread thread, jlong value) const;
  virtual jvmtiError forceEarlyReturnFloat(jvmtiEnv* env, jthread thread, jfloat value) const;
  virtual jvmtiError forceEarlyReturnDouble(jvmtiEnv* env, jthread thread, jdouble value) const;
  virtual jvmtiError forceEarlyReturnVoid(jvmtiEnv* env, jthread thread) const;
  virtual jvmtiError getVersionNumber(jvmtiEnv* env, jint* version_ptr) const;
  virtual jvmtiError getCapabilities(jvmtiEnv* env, jvmtiCapabilities* capabilities_ptr) const;
  virtual jvmtiError getSourceDebugExtension(jvmtiEnv* env, jclass klass, char** source_debug_extension_ptr) const;
  virtual jvmtiError getTag(jvmtiEnv* env, jobject object, jlong* tag_ptr) const;
  virtual jvmtiError setTag(jvmtiEnv* env, jobject object, jlong tag) const;
  virtual jvmtiError forceGarbageCollection(jvmtiEnv* env) const;
  virtual jvmtiError iterateOverObjectsReachableFromObject(jvmtiEnv* env, jobject object, jvmtiObjectReferenceCallback object_reference_callback, const void* user_data) const;
  virtual jvmtiError iterateOverReachableObjects(jvmtiEnv* env, jvmtiHeapRootCallback heap_root_callback, jvmtiStackReferenceCallback stack_ref_callback, jvmtiObjectReferenceCallback object_ref_callback, const void* user_data) const;
  virtual jvmtiError iterateOverHeap(jvmtiEnv* env, jvmtiHeapObjectFilter object_filter, jvmtiHeapObjectCallback heap_object_callback, const void* user_data) const;
  virtual jvmtiError iterateOverInstancesOfClass(jvmtiEnv* env, jclass klass, jvmtiHeapObjectFilter object_filter, jvmtiHeapObjectCallback heap_object_callback, const void* user_data) const;
  virtual jvmtiError getObjectsWithTags(jvmtiEnv* env, jint tag_count, const jlong* tags, jint* count_ptr, jobject** object_result_ptr, jlong** tag_result_ptr) const;
  virtual jvmtiError followReferences(jvmtiEnv* env, jint heap_filter, jclass klass, jobject initial_object, const jvmtiHeapCallbacks* callbacks, const void* user_data) const;
  virtual jvmtiError iterateThroughHeap(jvmtiEnv* env, jint heap_filter, jclass klass, const jvmtiHeapCallbacks* callbacks, const void* user_data) const;
  virtual jvmtiError setJNIFunctionTable(jvmtiEnv* env, const jniNativeInterface* function_table) const;
  virtual jvmtiError getJNIFunctionTable(jvmtiEnv* env, jniNativeInterface** function_table) const;
  virtual jvmtiError setEventCallbacks(jvmtiEnv* env, const jvmtiEventCallbacks* callbacks, jint size_of_callbacks) const;
  virtual jvmtiError generateEvents(jvmtiEnv* env, jvmtiEvent event_type) const;
  virtual jvmtiError getExtensionFunctions(jvmtiEnv* env, jint* extension_count_ptr, jvmtiExtensionFunctionInfo** extensions) const;
  virtual jvmtiError getExtensionEvents(jvmtiEnv* env, jint* extension_count_ptr, jvmtiExtensionEventInfo** extensions) const;
  virtual jvmtiError setExtensionEventCallback(jvmtiEnv* env, jint extension_event_index, jvmtiExtensionEvent callback) const;
  virtual jvmtiError disposeEnvironment(jvmtiEnv* env) const;
  virtual jvmtiError getErrorName(jvmtiEnv* env, jvmtiError error, char** name_ptr) const;
  virtual jvmtiError getJLocationFormat(jvmtiEnv* env,jvmtiJlocationFormat* format_ptr) const;
  virtual jvmtiError getSystemProperties(jvmtiEnv* env, jint* count_ptr, char*** property_ptr) const;
  virtual jvmtiError getSystemProperty(jvmtiEnv* env, const char* property, char** value_ptr) const;
  virtual jvmtiError setSystemProperty(jvmtiEnv* env, const char* property, const char* value) const;
  virtual jvmtiError getPhase(jvmtiEnv* env, jvmtiPhase* phase_ptr) const;
  virtual jvmtiError getTimerInfo(jvmtiEnv* env, jvmtiTimerInfo* info_ptr) const;
  virtual jvmtiError getTime(jvmtiEnv* env, jlong* nanos_ptr) const;
  virtual jvmtiError getPotentialCapabilities(jvmtiEnv* env, jvmtiCapabilities* capabilities_ptr) const;
  virtual jvmtiError addCapabilities(jvmtiEnv* env, const jvmtiCapabilities* capabilities_ptr) const;
  virtual jvmtiError relinquishCapabilities(jvmtiEnv* env, const jvmtiCapabilities* capabilities_ptr) const;
  virtual jvmtiError getAvailableProcessors(jvmtiEnv* env, jint* processor_count_ptr) const;
  virtual jvmtiError getEnvironmentLocalStorage(jvmtiEnv* env, void** data_ptr) const;
  virtual jvmtiError setEnvironmentLocalStorage(jvmtiEnv* env, const void* data) const;
  virtual jvmtiError setVerboseFlag(jvmtiEnv* env, jvmtiVerboseFlag flag, jboolean value) const;
  virtual jvmtiError getObjectSize(jvmtiEnv* env, jobject object, jlong* size_ptr) const;
  virtual jvmtiError getLocalInstance(jvmtiEnv* env, jthread thread, jint depth, jobject* value_ptr) const;
  virtual jvmtiError getFrameLocation(jvmtiEnv* env, jthread thread, jint depth, jmethodID* method_ptr, jlocation* location_ptr) const;
  virtual jvmtiError notifyFramePop(jvmtiEnv* env, jthread thread, jint depth) const;
  virtual jvmtiError getLocalObject(jvmtiEnv* env, jthread thread, jint depth, jint slot, jobject* value_ptr) const;
  virtual jvmtiError getLocalInt(jvmtiEnv* env, jthread thread, jint depth, jint slot, jint* value_ptr) const;
  virtual jvmtiError getLocalLong(jvmtiEnv* env, jthread thread, jint depth, jint slot, jlong* value_ptr) const;
  virtual jvmtiError getLocalFloat(jvmtiEnv* env, jthread thread, jint depth, jint slot, jfloat* value_ptr) const;
  virtual jvmtiError getLocalDouble(jvmtiEnv* env, jthread thread, jint depth, jint slot, jdouble* value_ptr) const;
  virtual jvmtiError setLocalObject(jvmtiEnv* env, jthread thread, jint depth, jint slot, jobject value) const;
  virtual jvmtiError setLocalInt(jvmtiEnv* env, jthread thread, jint depth, jint slot, jint value) const;
  virtual jvmtiError setLocalLong(jvmtiEnv* env, jthread thread, jint depth, jint slot, jlong value) const;
  virtual jvmtiError setLocalFloat(jvmtiEnv* env, jthread thread, jint depth, jint slot, jfloat value) const;
  virtual jvmtiError setLocalDouble(jvmtiEnv* env, jthread thread, jint depth, jint slot, jdouble value) const;
  virtual jvmtiError getAllStackTraces(jvmtiEnv* env, jint max_frame_count, jvmtiStackInfo** stack_info_ptr, jint* thread_count_ptr) const;
  virtual jvmtiError getStackTrace(jvmtiEnv* env, jthread thread, jint start_depth, jint max_frame_count, jvmtiFrameInfo* frame_buffer, jint* count_ptr) const;
 };
}