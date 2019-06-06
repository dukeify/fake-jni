# fake-jni

JNI and JVMTI without the Jvm

## Cloning
```sh
git clone --depth 1 --recursive https://github.com/meme/fake-jni.git
```

## Usage
To include `fake-jni` in your CMake project, clone it into your project's root directory and add the following lines to your `CMakeLists.txt`:
```cmake
add_subdirectory(fake-jni)
target_link_libraries(fake-jni)
```
Then simply build as usual:
```sh
mkdir build
cd build
cmake ..
make -j`nproc`
```
To compile `fake-jni` with debugging enabled, add the make flag `-DFAKE_JNI_DEBUG`

## Goals
- Binary compatability with JNI 1.8
- Drop-in replacement for an actual JVM, allowing users to compile against `fake-jni` while retaining full functionality
- The definition and linking of C++ classes through `fake-jni` to act as proxies for Java classes, that would otherwise be running on an actual JVM
- Portability to other platforms and architectures

## Licensing
The JNI headers are licensed under the GPL, with the "classpath" exception, meaning that we are free to use and implement the JNI headers in `fake-jni`, however, any changes made to `include/jni.h` or `include/jni_md.h` will be made under the GPL, in complete compliance with the licensing. The rest of `fake-jni`, including implementations of other GPL licensed headers, are licensed under the GPLv3 license.
