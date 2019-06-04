# fake-jni

JNI without the DefaultJvm

##### Goals

- Be binary compatible with version 1.8 of the JNI (latest)
- Allow users to compile their JNI applications against `fake-jni` to remove the need for the JNI (memory constraints, etc.)
- Allow users to easily define Java classes to be used through the JNI using C++ classes with minimal
  definitions beyond the class definition
- Be highly portable (architecture-wise) and tight with dependencies

##### Non-goals
- Interceptors support
- Threading support
- GC utilities
- Reflection support

##### Licensing

The JNI headers are licensed under the GPL, with the "classpath" exception, this means that we are free to use the JNI headers in this project, and implement them, but any changes made to `include/jni.h` or `include/jni_md.h` must be made under the GPL, and the rest of the project (even the portions that use the `jni.h` header) are under the MIT license
