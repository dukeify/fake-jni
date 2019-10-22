# fake-jni

## Cloning
```sh
git clone --depth 1 --recursive https://github.com/meme/fake-jni.git
```

## Requirements
fake-jni has no runtime dependencies :)

## Build Requirements
 - `CMake` >= 3.8
 - `libdl`
 - `make` or `ninja`

## Usage
To include `fake-jni` in your CMake project, clone it into your project's root directory and add the following lines to your `CMakeLists.txt`:
```cmake
add_subdirectory(fake-jni)
target_link_libraries(fake-jni)
```

## Building
### Build Flags
| Flag | Usage | Default | Description |
| :- | :- | :- | :- |
| `BUILD_FAKE_JNI_TESTS` | `=[ON OFF]` | `ON` | Builds and runs the test suite |
| `BUILD_FAKE_JNI_EXAMPLES` | `=[ON OFF]` | `ON` | Builds and runs the examples |
| `BUILD_FAKE_JNI_DEBUG` | `=[ON OFF]` | `OFF` | Builds a debug release |
| `BUILD_FAKE_JNI_ASAN` | `=[ON OFF]` | `OFF` | Builds with ASAN |
| `FFI_CC` | `={DESIRED_C_COMPILER}` | `${CMAKE_C_COMPILER}` | Set the C compiler for `libffi` |
| `FFI_CXX` | `={DESIRED_CXX_COMPILER}` | `${CMAKE_CXX_COMPILER}` | Set the C++ compiler for `libffi` |
| `UNWIND_CC` | `={DESIRED_C_COMPILER}` | `${CMAKE_C_COMPILER}` | Set the C compiler for `libunwind` |
| `UNWIND_CXX` | `={DESIRED_CXX_COMPILER}` | `${CMAKE_CXX_COMPILER}` | Set the C++ compiler for `libunwind` | 

Simply build as usual:
```sh
mkdir build
cd build
env CC=clang CXX=clang++ cmake ..
make -j
```
or optionally build with ninja:
```sh
mkdir build
cd build
env CC=clang CXX=clang++ cmake -GNinja ..
ninja -j0
```

### Cross Compiling
To compile for another host you must set the following environment variables:
 - `CMAKE_CXX_COMPILER_TARGET` - The target architecture
 - `CC` - The C cross-compiler
 - `CXX` - The C++ cross-compiler
 
Optionally, you may also set the following variables:
 - `FFI_CC` - The C cross-compiler for `libffi`
 - `FFI_CXX` - The C++ cross-compiler for `libffi`
 - `UNWIND_CC` - The C cross-compiler for `libunwind`
 - `UNWIND_CXX` - The C++ cross-compiler for `libunwind`

## Goals
- Binary compatability with JNI 1.8
- Drop-in replacement for an actual JVM, allowing users to compile against `fake-jni` while retaining full functionality
- The definition and linking of C++ classes through `fake-jni` to act as proxies for Java classes, that would otherwise be running on an actual JVM
- Portability to other platforms and architectures

## Licensing
The JNI headers are licensed under the GPL, with the "classpath" exception, meaning that we are free to use and implement the JNI headers in `fake-jni`, however, any changes made to `include/jni.h` or `include/jni_md.h` will be made under the GPL, in complete compliance with the licensing. The rest of `fake-jni`, including implementations of other GPL licensed headers, are licensed under the GPLv3 license.
