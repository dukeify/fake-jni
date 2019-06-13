#pragma once

#include "common.h"

namespace FakeJni {
 inline jvmtiError JvmtiInterface::_SetEventNotificationMode(jvmtiEnv* env, jvmtiEventMode mode, jvmtiEvent event_type, jthread event_thread, ...) {
  _FETCH_JVMTI_ENV
  _JVMTI_DEBUG(SetEventNotificationMode)
  //TODO the c-varargs are reserved for future functionality, keep that in mind
  return ji->setEventNotificationMode(env, mode, event_type, event_thread);
 }
}