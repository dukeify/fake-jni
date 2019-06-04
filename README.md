# fake-jni

JNI and JVMTI without the Jvm

## Goals
- Binary compatability with JNI 1.8
- Drop-in replacement for an actual JVM, allowing users to compile against `fake-jni` while retaining full functionality
- The definition and linking of C++ classes through `fake-jni` to act as proxies for Java classes, that would otherwise be running on an actual JVM
- Portability to other platforms and architectures

## Licensing
The JNI headers are licensed under the GPL, with the "classpath" exception, meaning that we are free to use and implement the JNI headers in `fake-jni`, however, any changes made to `include/jni.h` or `include/jni_md.h` will be made under the GPL, in complete compliance with the licensing. The rest of `fake-jni`, including implementations of other GPL licensed headers, are licensed under the GPLv3 license.
