cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(compiler_flags "-g -DFAKE_JNI_DEBUG")
set(CMAKE_CXX_FLAGS ${compiler_flags})
set(CMAKE_C_FLAGS ${compiler_flags})
#SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")

add_library(fake-jni INTERFACE)

target_include_directories(fake-jni INTERFACE include)
target_compile_options(fake-jni INTERFACE -Wall -Wextra -pedantic -Wno-unused-parameter)

add_subdirectory(tests)
add_subdirectory(examples)
