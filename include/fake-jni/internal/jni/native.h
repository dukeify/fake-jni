#pragma once

#ifdef FAKE_JNI_DEBUG
#define _FETCH_JNI_ENV \
JniEnv *jenv = (JniEnv *)env;\
NativeInterface * const ni = (NativeInterface *)(env->functions);
#else
#define _FETCH_JNI_ENV \
NativeInterface * const ni = (NativeInterface *)(env->functions);
#endif

#ifdef FAKE_JNI_DEBUG
#define _NATIVE_DEBUG(name) fprintf(jenv->getVM().getLog(), "DEBUG: JNINativeInterface_::%s\n", #name);
#else
#define _NATIVE_DEBUG(name)
#endif