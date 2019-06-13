#pragma once

namespace FakeJni {
 //TODO implement
 jvmtiError JvmtiInterface::getOwnedMonitorInfo(jvmtiEnv* env, jthread thread, jint* owned_monitor_count_ptr, jobject** owned_monitors_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getOwnedMonitorInfo' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getCurrentContendedMonitor(jvmtiEnv* env, jthread thread, jobject* monitor_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getCurrentContendedMonitor' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::createRawMonitor(jvmtiEnv* env, const char* name, jrawMonitorID* monitor_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::createRawMonitor' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::destroyRawMonitor(jvmtiEnv* env, jrawMonitorID monitor) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::destroyRawMonitor' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::rawMonitorEnter(jvmtiEnv* env, jrawMonitorID monitor) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::rawMonitorEnter' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::rawMonitorExit(jvmtiEnv* env, jrawMonitorID monitor) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::rawMonitorExit' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::rawMonitorWait(jvmtiEnv* env, jrawMonitorID monitor, jlong millis) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::rawMonitorWait' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::rawMonitorNotify(jvmtiEnv* env, jrawMonitorID monitor) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::rawMonitorNotify' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::rawMonitorNotifyAll(jvmtiEnv* env, jrawMonitorID monitor) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::rawMonitorNotifyAll' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getObjectMonitorUsage(jvmtiEnv* env, jobject object, jvmtiMonitorUsage* info_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getObjectMonitorUsage' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getOwnedMonitorStackDepthInfo(jvmtiEnv* env, jthread thread, jint* monitor_info_count_ptr, jvmtiMonitorStackDepthInfo** monitor_info_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getOwnedMonitorStackDepthInfo' is unimplemented!");
 }


}