#include "fake-jni/internal/jvmti/jvmti.h"
#include "fake-jni/jvm.h"

namespace FakeJni {
 JvmtiInterface::JvmtiInterface(const Jvm& vm) :
  jvmtiInterface_1(),
  vm(const_cast<Jvm &>(vm))
 {
  reserved1
   = reserved3
   = reserved40
   = reserved67
   = reserved94
   = reserved95
   = reserved96
   = reserved97
   = reserved98
   = reserved99
   = reserved105
   = reserved113
   = reserved117
   = reserved118
   = reserved119
   = reserved141
   = nullptr;
  SetEventNotificationMode = &JvmtiInterface::_SetEventNotificationMode;
  GetAllThreads = [](jvmtiEnv* env, jint* threads_count_ptr, jthread** threads_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetAllThreads)
   return ji->getAllThreads(env, threads_count_ptr, threads_ptr);
  };
  SuspendThread = [](jvmtiEnv* env, jthread thread) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SuspendThread)
   return ji->suspendThread(env, thread);
  };
  ResumeThread = [](jvmtiEnv* env, jthread thread) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(ResumeThread)
   return ji->resumeThread(env, thread);
  };
  StopThread = [](jvmtiEnv* env, jthread thread, jobject exception) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(StopThread)
   return ji->stopThread(env, thread, exception);
  };
  InterruptThread = [](jvmtiEnv* env, jthread thread) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(InterruptThread)
   return ji->interruptThread(env, thread);
  };
  GetThreadInfo = [](jvmtiEnv* env, jthread thread, jvmtiThreadInfo* info_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetThreadInfo)
   return ji->getThreadInfo(env, thread, info_ptr);
  };
  GetOwnedMonitorInfo = [](jvmtiEnv* env, jthread thread, jint* owned_monitor_count_ptr, jobject** owned_monitors_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetOwnedMonitorInfo)
   return ji->getOwnedMonitorInfo(env, thread, owned_monitor_count_ptr, owned_monitors_ptr);
  };
  GetCurrentContendedMonitor = [](jvmtiEnv* env, jthread thread, jobject* monitor_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetCurrentContendedMonitor)
   return ji->getCurrentContendedMonitor(env, thread, monitor_ptr);
  };
  RunAgentThread = [](jvmtiEnv* env, jthread thread, jvmtiStartFunction proc, const void* arg, jint priority) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(RunAgentThread)
   return ji->runAgentThread(env, thread, proc, arg, priority);
  };
  GetTopThreadGroups = [](jvmtiEnv* env, jint* group_count_ptr, jthreadGroup** groups_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetTopThreadGroups)
   return ji->getTopThreadGroups(env, group_count_ptr, groups_ptr);
  };
  GetThreadGroupInfo = [](jvmtiEnv* env, jthreadGroup group, jvmtiThreadGroupInfo* info_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetThreadGroupInfo)
   return ji->getThreadGroupInfo(env, group, info_ptr);
  };
  GetThreadGroupChildren = [](jvmtiEnv* env, jthreadGroup group, jint* thread_count_ptr, jthread** threads_ptr, jint* group_count_ptr, jthreadGroup** groups_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetThreadGroupChildren)
   return ji->getThreadGroupChildren(env, group, thread_count_ptr, threads_ptr, group_count_ptr, groups_ptr);
  };
  GetFrameCount = [](jvmtiEnv* env, jthread thread, jint* count_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetFrameCount)
   return ji->getFrameCount(env, thread, count_ptr);
  };
  GetThreadState = [](jvmtiEnv* env, jthread thread, jint* thread_state_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetThreadState)
   return ji->getThreadState(env, thread, thread_state_ptr);
  };
  GetCurrentThread = [](jvmtiEnv* env, jthread* thread_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetCurrentThread)
   return ji->getCurrentThread(env, thread_ptr);
  };
  GetFrameLocation = [](jvmtiEnv* env, jthread thread, jint depth, jmethodID* method_ptr, jlocation* location_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetFrameLocation)
   return ji->getFrameLocation(env, thread, depth, method_ptr, location_ptr);
  };
  NotifyFramePop = [](jvmtiEnv* env, jthread thread, jint depth) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(NotifyFramePop)
   return ji->notifyFramePop(env, thread, depth);
  };
  GetLocalObject = [](jvmtiEnv* env, jthread thread, jint depth, jint slot, jobject* value_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetLocalObject)
   return ji->getLocalObject(env, thread, depth, slot, value_ptr);
  };
  GetLocalInt = [](jvmtiEnv* env, jthread thread, jint depth, jint slot, jint* value_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetLocalInt)
   return ji->getLocalInt(env, thread, depth, slot, value_ptr);
  };
  GetLocalLong = [](jvmtiEnv* env, jthread thread, jint depth, jint slot, jlong* value_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetLocalLong)
   return ji->getLocalLong(env, thread, depth, slot, value_ptr);
  };
  GetLocalFloat = [](jvmtiEnv* env, jthread thread, jint depth, jint slot, jfloat* value_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetLocalFloat)
   return ji->getLocalFloat(env, thread, depth, slot, value_ptr);
  };
  GetLocalDouble = [](jvmtiEnv* env, jthread thread, jint depth, jint slot, jdouble* value_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetLocalDouble)
   return ji->getLocalDouble(env, thread, depth, slot, value_ptr);
  };
  SetLocalObject = [](jvmtiEnv* env, jthread thread, jint depth, jint slot, jobject value) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetLocalObject)
   return ji->setLocalObject(env, thread, depth, slot, value);
  };
  SetLocalInt = [](jvmtiEnv* env, jthread thread, jint depth, jint slot, jint value) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetLocalInt)
   return ji->setLocalInt(env, thread, depth, slot, value);
  };
  SetLocalLong = [](jvmtiEnv* env, jthread thread, jint depth, jint slot, jlong value) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetLocalLong)
   return ji->setLocalLong(env, thread, depth, slot, value);
  };
  SetLocalFloat = [](jvmtiEnv* env, jthread thread, jint depth, jint slot, jfloat value) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetLocalFloat)
   return ji->setLocalFloat(env, thread, depth, slot, value);
  };
  SetLocalDouble = [](jvmtiEnv* env, jthread thread, jint depth, jint slot, jdouble value) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetLocalDouble)
   return ji->setLocalDouble(env, thread, depth, slot, value);
  };
  CreateRawMonitor = [](jvmtiEnv* env, const char* name, jrawMonitorID* monitor_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(CreateRawMonitor)
   return ji->createRawMonitor(env, name, monitor_ptr);
  };
  DestroyRawMonitor = [](jvmtiEnv* env, jrawMonitorID monitor) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(DestroyRawMonitor)
   return ji->destroyRawMonitor(env, monitor);
  };
  RawMonitorEnter = [](jvmtiEnv* env, jrawMonitorID monitor) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(RawMonitorEnter)
   return ji->rawMonitorEnter(env, monitor);
  };
  RawMonitorExit = [](jvmtiEnv* env, jrawMonitorID monitor) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(RawMonitorExit)
   return ji->rawMonitorExit(env, monitor);
  };
  RawMonitorWait = [](jvmtiEnv* env, jrawMonitorID monitor, jlong millis) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(RawMonitorWait)
   return ji->rawMonitorWait(env, monitor, millis);
  };
  RawMonitorNotify = [](jvmtiEnv* env, jrawMonitorID monitor) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(RawMonitorNotify)
   return ji->rawMonitorNotify(env, monitor);
  };
  RawMonitorNotifyAll = [](jvmtiEnv* env, jrawMonitorID monitor) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(RawMonitorNotifyAll)
   return ji->rawMonitorNotifyAll(env, monitor);
  };
  SetBreakpoint = [](jvmtiEnv* env, jmethodID method, jlocation location) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetBreakpoint)
   return ji->setBreakpoint(env, method, location);
  };
  ClearBreakpoint = [](jvmtiEnv* env, jmethodID method, jlocation location) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(ClearBreakpoint)
   return ji->clearBreakpoint(env, method, location);
  };
  SetFieldAccessWatch = [](jvmtiEnv* env, jclass klass, jfieldID field) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetFieldAccessWatch)
   return ji->setFieldAccessWatch(env, klass, field);
  };
  ClearFieldAccessWatch = [](jvmtiEnv* env, jclass klass, jfieldID field) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(ClearFieldAccessWatch)
   return ji->clearFieldAccessWatch(env, klass, field);
  };
  SetFieldModificationWatch = [](jvmtiEnv* env, jclass klass, jfieldID field) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetFieldModificationWatch)
   return ji->setFieldModificationWatch(env, klass, field);
  };
  ClearFieldModificationWatch = [](jvmtiEnv* env, jclass klass, jfieldID field) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(ClearFieldModificationWatch)
   return ji->clearFieldModificationWatch(env, klass, field);
  };
  IsModifiableClass = [](jvmtiEnv* env, jclass klass, jboolean* is_modifiable_class_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(IsModifiableClass)
   return ji->isModifiableClass(env, klass, is_modifiable_class_ptr);
  };
  Allocate = [](jvmtiEnv* env, jlong size, unsigned char** mem_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(Allocate)
   return ji->allocate(env, size, mem_ptr);
  };
  Deallocate = [](jvmtiEnv* env, unsigned char* mem) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(Deallocate)
   return ji->deallocate(env, mem);
  };
  GetClassSignature = [](jvmtiEnv* env, jclass klass, char** signature_ptr, char** generic_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetClassSignature)
   return ji->getClassSignature(env, klass, signature_ptr, generic_ptr);
  };
  GetClassStatus = [](jvmtiEnv* env, jclass klass, jint* status_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetClassStatus)
   return ji->getClassStatus(env, klass, status_ptr);
  };
  GetSourceFileName = [](jvmtiEnv* env, jclass klass, char** source_name_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetSourceFileName)
   return ji->getSourceFileName(env, klass, source_name_ptr);
  };
  GetClassModifiers = [](jvmtiEnv* env, jclass klass, jint* modifiers_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetClassModifiers)
   return ji->getClassModifiers(env, klass, modifiers_ptr);
  };
  GetClassMethods = [](jvmtiEnv* env, jclass klass, jint* method_count_ptr, jmethodID** methods_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetClassMethods)
   return ji->getClassMethods(env, klass, method_count_ptr, methods_ptr);
  };
  GetClassFields = [](jvmtiEnv* env, jclass klass, jint* field_count_ptr, jfieldID** fields_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetClassFields)
   return ji->getClassFields(env, klass, field_count_ptr, fields_ptr);
  };
  GetImplementedInterfaces = [](jvmtiEnv* env, jclass klass, jint* interface_count_ptr, jclass** interfaces_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetImplementedInterfaces)
   return ji->getImplementedInterfaces(env, klass, interface_count_ptr, interfaces_ptr);
  };
  IsInterface = [](jvmtiEnv* env, jclass klass, jboolean* is_interface_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(IsInterface)
   return ji->isInterface(env, klass, is_interface_ptr);
  };
  IsArrayClass = [](jvmtiEnv* env, jclass klass, jboolean* is_array_class_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(IsArrayClass)
   return ji->isArrayClass(env, klass, is_array_class_ptr);
  };
  GetClassLoader = [](jvmtiEnv* env, jclass klass, jobject* classloader_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetClassLoader)
   return ji->getClassLoader(env, klass, classloader_ptr);
  };
  GetObjectHashCode = [](jvmtiEnv* env, jobject object, jint* hash_code_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetObjectHashCode)
   return ji->getObjectHashCode(env, object, hash_code_ptr);
  };
  GetObjectMonitorUsage = [](jvmtiEnv* env, jobject object, jvmtiMonitorUsage* info_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetObjectMonitorUsage)
   return ji->getObjectMonitorUsage(env, object, info_ptr);
  };
  GetFieldName = [](jvmtiEnv* env, jclass klass, jfieldID field, char** name_ptr, char** signature_ptr, char** generic_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetFieldName)
   return ji->getFieldName(env, klass, field, name_ptr, signature_ptr, generic_ptr);
  };
  GetFieldDeclaringClass = [](jvmtiEnv* env, jclass klass, jfieldID field, jclass* declaring_class_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetFieldDeclaringClass)
   return ji->getFieldDeclaringClass(env, klass, field, declaring_class_ptr);
  };
  GetFieldModifiers = [](jvmtiEnv* env, jclass klass, jfieldID field, jint* modifiers_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetFieldModifiers)
   return ji->getFieldModifiers(env, klass, field, modifiers_ptr);
  };
  IsFieldSynthetic = [](jvmtiEnv* env, jclass klass, jfieldID field, jboolean* is_synthetic_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(IsFieldSynthetic)
   return ji->isFieldSynthetic(env, klass, field, is_synthetic_ptr);
  };
  GetMethodName = [](jvmtiEnv* env, jmethodID method, char** name_ptr, char** signature_ptr, char** generic_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetMethodName)
   return ji->getMethodName(env, method, name_ptr, signature_ptr, generic_ptr);
  };
  GetMethodDeclaringClass = [](jvmtiEnv* env, jmethodID method, jclass* declaring_class_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetMethodDeclaringClass)
   return ji->getMethodDeclaringClass(env, method, declaring_class_ptr);
  };
  GetMethodModifiers = [](jvmtiEnv* env, jmethodID method, jint* modifiers_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetMethodModifiers)
   return ji->getMethodModifiers(env, method, modifiers_ptr);
  };
  GetMaxLocals = [](jvmtiEnv* env, jmethodID method, jint* max_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetMaxLocals)
   return ji->getMaxLocals(env, method, max_ptr);
  };
  GetArgumentsSize = [](jvmtiEnv* env, jmethodID method, jint* size_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetArgumentsSize)
   return ji->getArgumentsSize(env, method, size_ptr);
  };
  GetLineNumberTable = [](jvmtiEnv* env, jmethodID method, jint* entry_count_ptr, jvmtiLineNumberEntry** table_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetLineNumberTable)
   return ji->getLineNumberTable(env, method, entry_count_ptr, table_ptr);
  };
  GetMethodLocation = [](jvmtiEnv* env, jmethodID method, jlocation* start_location_ptr, jlocation* end_location_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetMethodLocation)
   return ji->getMethodLocation(env, method, start_location_ptr, end_location_ptr);
  };
  GetLocalVariableTable = [](jvmtiEnv* env, jmethodID method, jint* entry_count_ptr, jvmtiLocalVariableEntry** table_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetLocalVariableTable)
   return ji->getLocalVariableTable(env, method, entry_count_ptr, table_ptr);
  };
  SetNativeMethodPrefix = [](jvmtiEnv* env, const char* prefix) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetNativeMethodPrefix)
   return ji->setNativeMethodPrefix(env, prefix);
  };
  SetNativeMethodPrefixes = [](jvmtiEnv* env, jint prefix_count, char** prefixes) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetNativeMethodPrefixes)
   return ji->setNativeMethodPrefixes(env, prefix_count, prefixes);
  };
  GetBytecodes = [](jvmtiEnv* env, jmethodID method, jint* bytecode_count_ptr, unsigned char** bytecodes_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetBytecodes)
   return ji->getBytecodes(env, method, bytecode_count_ptr, bytecodes_ptr);
  };
  IsMethodNative = [](jvmtiEnv* env, jmethodID method, jboolean* is_native_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(IsMethodNative)
   return ji->isMethodNative(env, method, is_native_ptr);
  };
  IsMethodSynthetic = [](jvmtiEnv* env, jmethodID method, jboolean* is_synthetic_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(IsMethodSynthetic)
   return ji->isMethodSynthetic(env, method, is_synthetic_ptr);
  };
  GetLoadedClasses = [](jvmtiEnv* env, jint* class_count_ptr, jclass** classes_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetLoadedClasses)
   return ji->getLoadedClasses(env, class_count_ptr, classes_ptr);
  };
  GetClassLoaderClasses = [](jvmtiEnv* env, jobject initiating_loader, jint* class_count_ptr, jclass** classes_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetClassLoaderClasses)
   return ji->getClassLoaderClasses(env, initiating_loader, class_count_ptr, classes_ptr);
  };
  PopFrame = [](jvmtiEnv* env, jthread thread) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(PopFrame)
   return ji->popFrame(env, thread);
  };
  ForceEarlyReturnObject = [](jvmtiEnv* env, jthread thread, jobject value) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(ForceEarlyReturnObject)
   return ji->forceEarlyReturnObject(env, thread, value);
  };
  ForceEarlyReturnInt = [](jvmtiEnv* env, jthread thread, jint value) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(ForceEarlyReturnInt)
   return ji->forceEarlyReturnInt(env, thread, value);
  };
  ForceEarlyReturnLong = [](jvmtiEnv* env, jthread thread, jlong value) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(ForceEarlyReturnLong)
   return ji->forceEarlyReturnLong(env, thread, value);
  };
  ForceEarlyReturnFloat = [](jvmtiEnv* env, jthread thread, jfloat value) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(ForceEarlyReturnFloat)
   return ji->forceEarlyReturnFloat(env, thread, value);
  };
  ForceEarlyReturnDouble = [](jvmtiEnv* env, jthread thread, jdouble value) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(ForceEarlyReturnDouble)
   return ji->forceEarlyReturnDouble(env, thread, value);
  };
  ForceEarlyReturnVoid = [](jvmtiEnv* env, jthread thread) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(ForceEarlyReturnVoid)
   return ji->forceEarlyReturnVoid(env, thread);
  };
  RedefineClasses = [](jvmtiEnv* env, jint class_count, const jvmtiClassDefinition* class_definitions) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(RedefineClasses)
   return ji->redefineClasses(env, class_count, class_definitions);
  };
  GetVersionNumber = [](jvmtiEnv* env, jint* version_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetVersionNumber)
   return ji->getVersionNumber(env, version_ptr);
  };
  GetCapabilities = [](jvmtiEnv* env, jvmtiCapabilities* capabilities_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetCapabilities)
   return ji->getCapabilities(env, capabilities_ptr);
  };
  GetSourceDebugExtension = [](jvmtiEnv* env, jclass klass, char** source_debug_extension_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetSourceDebugExtension)
   return ji->getSourceDebugExtension(env, klass, source_debug_extension_ptr);
  };
  IsMethodObsolete = [](jvmtiEnv* env, jmethodID method, jboolean* is_obsolete_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(IsMethodObsolete)
   return ji->isMethodObsolete(env, method, is_obsolete_ptr);
  };
  SuspendThreadList = [](jvmtiEnv* env, jint request_count, const jthread* request_list, jvmtiError* results) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SuspendThreadList)
   return ji->suspendThreadList(env, request_count, request_list, results);
  };
  ResumeThreadList = [](jvmtiEnv* env, jint request_count, const jthread* request_list, jvmtiError* results) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(ResumeThreadList)
   return ji->resumeThreadList(env, request_count, request_list, results);
  };
  GetAllStackTraces = [](jvmtiEnv* env, jint max_frame_count, jvmtiStackInfo** stack_info_ptr, jint* thread_count_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetAllStackTraces)
   return ji->getAllStackTraces(env, max_frame_count, stack_info_ptr, thread_count_ptr);
  };
  GetThreadListStackTraces = [](jvmtiEnv* env, jint thread_count, const jthread* thread_list, jint max_frame_count, jvmtiStackInfo** stack_info_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetThreadListStackTraces)
   return ji->getThreadListStackTraces(env, thread_count, thread_list, max_frame_count, stack_info_ptr);
  };
  GetThreadLocalStorage = [](jvmtiEnv* env, jthread thread, void** data_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetThreadLocalStorage)
   return ji->getThreadLocalStorage(env, thread, data_ptr);
  };
  SetThreadLocalStorage = [](jvmtiEnv* env, jthread thread, const void* data) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetThreadLocalStorage)
   return ji->setThreadLocalStorage(env, thread, data);
  };
  GetStackTrace = [](jvmtiEnv* env, jthread thread, jint start_depth, jint max_frame_count, jvmtiFrameInfo* frame_buffer, jint* count_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetStackTrace)
   return ji->getStackTrace(env, thread, start_depth, max_frame_count, frame_buffer, count_ptr);
  };
  GetTag = [](jvmtiEnv* env, jobject object, jlong* tag_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetTag)
   return ji->getTag(env, object, tag_ptr);
  };
  SetTag = [](jvmtiEnv* env, jobject object, jlong tag) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetTag)
   return ji->setTag(env, object, tag);
  };
  ForceGarbageCollection = [](jvmtiEnv* env) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(ForceGarbageCollection)
   return ji->forceGarbageCollection(env);
  };
  IterateOverObjectsReachableFromObject = [](jvmtiEnv* env, jobject object, jvmtiObjectReferenceCallback object_reference_callback, const void* user_data) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(IterateOverObjectsReachableFromObject)
   return ji->iterateOverObjectsReachableFromObject(env, object, object_reference_callback, user_data);
  };
  IterateOverReachableObjects = [](jvmtiEnv* env, jvmtiHeapRootCallback heap_root_callback, jvmtiStackReferenceCallback stack_ref_callback, jvmtiObjectReferenceCallback object_ref_callback, const void* user_data) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(IterateOverReachableObjects)
   return ji->iterateOverReachableObjects(env, heap_root_callback, stack_ref_callback, object_ref_callback, user_data);
  };
  IterateOverHeap = [](jvmtiEnv* env, jvmtiHeapObjectFilter object_filter, jvmtiHeapObjectCallback heap_object_callback, const void* user_data) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(IterateOverHeap)
   return ji->iterateOverHeap(env, object_filter, heap_object_callback, user_data);
  };
  IterateOverInstancesOfClass = [](jvmtiEnv* env, jclass klass, jvmtiHeapObjectFilter object_filter, jvmtiHeapObjectCallback heap_object_callback, const void* user_data) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(IterateOverInstancesOfClass)
   return ji->iterateOverInstancesOfClass(env, klass, object_filter, heap_object_callback, user_data);
  };
  GetObjectsWithTags = [](jvmtiEnv* env, jint tag_count, const jlong* tags, jint* count_ptr, jobject** object_result_ptr, jlong** tag_result_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetObjectsWithTags)
   return ji->getObjectsWithTags(env, tag_count, tags, count_ptr, object_result_ptr, tag_result_ptr);
  };
  FollowReferences = [](jvmtiEnv* env, jint heap_filter, jclass klass, jobject initial_object, const jvmtiHeapCallbacks* callbacks, const void* user_data) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(FollowReferences)
   return ji->followReferences(env, heap_filter, klass, initial_object, callbacks, user_data);
  };
  IterateThroughHeap = [](jvmtiEnv* env, jint heap_filter, jclass klass, const jvmtiHeapCallbacks* callbacks, const void* user_data) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(IterateThroughHeap)
   return ji->iterateThroughHeap(env, heap_filter, klass, callbacks, user_data);
  };
  SetJNIFunctionTable = [](jvmtiEnv* env, const jniNativeInterface* function_table) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetJNIFunctionTable)
   return ji->setJNIFunctionTable(env, function_table);
  };
  GetJNIFunctionTable = [](jvmtiEnv* env, jniNativeInterface** function_table) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetJNIFunctionTable)
   return ji->getJNIFunctionTable(env, function_table);
  };
  SetEventCallbacks = [](jvmtiEnv* env, const jvmtiEventCallbacks* callbacks, jint size_of_callbacks) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetEventCallbacks)
   return ji->setEventCallbacks(env, callbacks, size_of_callbacks);
  };
  GenerateEvents = [](jvmtiEnv* env, jvmtiEvent event_type) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GenerateEvents)
   return ji->generateEvents(env, event_type);
  };
  GetExtensionFunctions = [](jvmtiEnv* env, jint* extension_count_ptr, jvmtiExtensionFunctionInfo** extensions) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetExtensionFunctions)
   return ji->getExtensionFunctions(env, extension_count_ptr, extensions);
  };
  GetExtensionEvents = [](jvmtiEnv* env, jint* extension_count_ptr, jvmtiExtensionEventInfo** extensions) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetExtensionEvents)
   return ji->getExtensionEvents(env, extension_count_ptr, extensions);
  };
  SetExtensionEventCallback = [](jvmtiEnv* env, jint extension_event_index, jvmtiExtensionEvent callback) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetExtensionEventCallback)
   return ji->setExtensionEventCallback(env, extension_event_index, callback);
  };
  DisposeEnvironment = [](jvmtiEnv* env) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(DisposeEnvironment)
   return ji->disposeEnvironment(env);
  };
  GetErrorName = [](jvmtiEnv* env, jvmtiError error, char** name_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetErrorName)
   return ji->getErrorName(env, error, name_ptr);
  };
  GetJLocationFormat = [](jvmtiEnv* env, jvmtiJlocationFormat* format_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetJLocationFormat)
   return ji->getJLocationFormat(env, format_ptr);
  };
  GetSystemProperties = [](jvmtiEnv* env, jint* count_ptr, char*** property_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetSystemProperties)
   return ji->getSystemProperties(env, count_ptr, property_ptr);
  };
  GetSystemProperty = [](jvmtiEnv* env, const char* property, char** value_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetSystemProperty)
   return ji->getSystemProperty(env, property, value_ptr);
  };
  SetSystemProperty = [](jvmtiEnv* env, const char* property, const char* value) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetSystemProperty)
   return ji->setSystemProperty(env, property, value);
  };
  GetPhase = [](jvmtiEnv* env, jvmtiPhase* phase_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetPhase)
   return ji->getPhase(env, phase_ptr);
  };
  GetCurrentThreadCpuTimerInfo = [](jvmtiEnv* env, jvmtiTimerInfo* info_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetCurrentThreadCpuTimerInfo)
   return ji->getCurrentThreadCpuTimerInfo(env, info_ptr);
  };
  GetCurrentThreadCpuTime = [](jvmtiEnv* env, jlong* nanos_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetCurrentThreadCpuTime)
   return ji->getCurrentThreadCpuTime(env, nanos_ptr);
  };
  GetThreadCpuTimerInfo = [](jvmtiEnv* env, jvmtiTimerInfo* info_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetThreadCpuTimerInfo)
   return ji->getThreadCpuTimerInfo(env, info_ptr);
  };
  GetThreadCpuTime = [](jvmtiEnv* env, jthread thread, jlong* nanos_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetThreadCpuTime)
   return ji->getThreadCpuTime(env, thread, nanos_ptr);
  };
  GetTimerInfo = [](jvmtiEnv* env, jvmtiTimerInfo* info_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetTimerInfo)
   return ji->getTimerInfo(env, info_ptr);
  };
  GetTime = [](jvmtiEnv* env, jlong* nanos_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetTime)
   return ji->getTime(env, nanos_ptr);
  };
  GetPotentialCapabilities = [](jvmtiEnv* env, jvmtiCapabilities* capabilities_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetPotentialCapabilities)
   return ji->getPotentialCapabilities(env, capabilities_ptr);
  };
  AddCapabilities = [](jvmtiEnv* env, const jvmtiCapabilities* capabilities_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(AddCapabilities)
   return ji->addCapabilities(env, capabilities_ptr);
  };
  RelinquishCapabilities = [](jvmtiEnv* env, const jvmtiCapabilities* capabilities_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(RelinquishCapabilities)
   return ji->relinquishCapabilities(env, capabilities_ptr);
  };
  GetAvailableProcessors = [](jvmtiEnv* env, jint* processor_count_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetAvailableProcessors)
   return ji->getAvailableProcessors(env, processor_count_ptr);
  };
  GetClassVersionNumbers = [](jvmtiEnv* env, jclass klass, jint* minor_version_ptr, jint* major_version_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetClassVersionNumbers)
   return ji->getClassVersionNumbers(env, klass, minor_version_ptr, major_version_ptr);
  };
  GetConstantPool = [](jvmtiEnv* env, jclass klass, jint* constant_pool_count_ptr, jint* constant_pool_byte_count_ptr, unsigned char** constant_pool_bytes_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetConstantPool)
   return ji->getConstantPool(env, klass, constant_pool_count_ptr, constant_pool_byte_count_ptr, constant_pool_bytes_ptr);
  };
  GetEnvironmentLocalStorage = [](jvmtiEnv* env, void** data_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetEnvironmentLocalStorage)
   return ji->getEnvironmentLocalStorage(env, data_ptr);
  };
  SetEnvironmentLocalStorage = [](jvmtiEnv* env, const void* data) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetEnvironmentLocalStorage)
   return ji->setEnvironmentLocalStorage(env, data);
  };
  AddToBootstrapClassLoaderSearch = [](jvmtiEnv* env, const char* segment) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(AddToBootstrapClassLoaderSearch)
   return ji->addToBootstrapClassLoaderSearch(env, segment);
  };
  SetVerboseFlag = [](jvmtiEnv* env, jvmtiVerboseFlag flag, jboolean value) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(SetVerboseFlag)
   return ji->setVerboseFlag(env, flag, value);
  };
  AddToSystemClassLoaderSearch = [](jvmtiEnv* env, const char* segment) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(AddToSystemClassLoaderSearch)
   return ji->addToSystemClassLoaderSearch(env, segment);
  };
  RetransformClasses = [](jvmtiEnv* env, jint class_count, const jclass* classes) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(RetransformClasses)
   return ji->retransformClasses(env, class_count, classes);
  };
  GetOwnedMonitorStackDepthInfo = [](jvmtiEnv* env, jthread thread, jint* monitor_info_count_ptr, jvmtiMonitorStackDepthInfo** monitor_info_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetOwnedMonitorStackDepthInfo)
   return ji->getOwnedMonitorStackDepthInfo(env, thread, monitor_info_count_ptr, monitor_info_ptr);
  };
  GetObjectSize = [](jvmtiEnv* env, jobject object, jlong* size_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetObjectSize)
   return ji->getObjectSize(env, object, size_ptr);
  };
  GetLocalInstance = [](jvmtiEnv* env, jthread thread, jint depth, jobject* value_ptr) -> jvmtiError {
   _FETCH_JVMTI_ENV
   _JVMTI_DEBUG(GetLocalInstance)
   return ji->getLocalInstance(env, thread, depth, value_ptr);
  };
 }
}