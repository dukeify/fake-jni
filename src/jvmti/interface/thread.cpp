#include "fake-jni/jvm.h"

namespace FakeJni {
//TODO implement
 jvmtiError JvmtiInterface::getAllThreads(jvmtiEnv* env, jint* threads_count_ptr, jthread** threads_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getAllThreads' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::suspendThread(jvmtiEnv* env, jthread thread) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::suspendThread' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::resumeThread(jvmtiEnv* env, jthread thread) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::resumeThread' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::stopThread(jvmtiEnv* env, jthread thread, jobject exception) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::stopThread' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::interruptThread(jvmtiEnv* env, jthread thread) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::interruptThread' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getThreadInfo(jvmtiEnv* env, jthread thread, jvmtiThreadInfo* info_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getThreadInfo' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::runAgentThread(jvmtiEnv* env, jthread thread, jvmtiStartFunction proc, const void* arg, jint priority) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::runAgentThread' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getTopThreadGroups(jvmtiEnv* env, jint* group_count_ptr, jthreadGroup** groups_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getTopThreadGroups' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getThreadGroupInfo(jvmtiEnv* env, jthreadGroup group, jvmtiThreadGroupInfo* info_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getThreadGroupInfo' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getThreadGroupChildren(jvmtiEnv* env, jthreadGroup group, jint* thread_count_ptr, jthread** threads_ptr, jint* group_count_ptr, jthreadGroup** groups_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getThreadGroupChildren' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getFrameCount(jvmtiEnv* env, jthread thread, jint* count_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getFrameCount' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getThreadState(jvmtiEnv* env, jthread thread, jint* thread_state_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getThreadState' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getCurrentThread(jvmtiEnv* env, jthread* thread_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getCurrentThread' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::suspendThreadList(jvmtiEnv* env, jint request_count, const jthread* request_list, jvmtiError* results) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::suspendThreadList' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::resumeThreadList(jvmtiEnv* env, jint request_count, const jthread* request_list, jvmtiError* results) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::resumeThreadList' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getThreadListStackTraces(jvmtiEnv* env, jint thread_count, const jthread* thread_list, jint max_frame_count, jvmtiStackInfo** stack_info_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getThreadListStackTraces' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getThreadLocalStorage(jvmtiEnv* env, jthread thread, void** data_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getThreadLocalStorage' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::setThreadLocalStorage(jvmtiEnv* env, jthread thread, const void* data) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::setThreadLocalStorage' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getCurrentThreadCpuTimerInfo(jvmtiEnv* env, jvmtiTimerInfo* info_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getCurrentThreadCpuTimerInfo' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getCurrentThreadCpuTime(jvmtiEnv* env, jlong* nanos_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getCurrentThreadCpuTime' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getThreadCpuTimerInfo(jvmtiEnv* env, jvmtiTimerInfo* info_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getThreadCpuTimerInfo' is unimplemented!");
 }

//TODO implement
 jvmtiError JvmtiInterface::getThreadCpuTime(jvmtiEnv* env, jthread thread, jlong* nanos_ptr) const {
  throw std::runtime_error("FATAL: 'jvmtiInterface_1_::getThreadCpuTime' is unimplemented!");
 }


}