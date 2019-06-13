#pragma once

#ifdef FAKE_JNI_DEBUG
#define _FETCH_JVMTI_ENV \
JvmtiEnv *jenv = (JvmtiEnv *)env;\
JvmtiInterface * const ji = (JvmtiInterface *)(env->functions);

#define _JVMTI_DEBUG(name) fprintf(jenv->getVM()->getLog(), "DEBUG: jvmtiInterface_1_::%s\n", #name);
#else
#define _FETCH_JVMTI_ENV \
JvmtiInterface * const ji = (JvmtiInterface *)(env->functions);

#define _JVMTI_DEBUG(name)
#endif