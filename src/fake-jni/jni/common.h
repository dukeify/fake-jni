#pragma once

//No need to store an extra stack variable if debugging is disabled
#ifdef FAKE_JNI_DEBUG
#define _FETCH_JNI_ENV \
JniEnv *jenv = (JniEnv *)env;\
NativeInterface * const ni = (NativeInterface *)(env->functions);
#else
#define _FETCH_JNI_ENV \
NativeInterface * const ni = (NativeInterface *)(env->functions);
#endif