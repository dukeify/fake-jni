cmake_minimum_required(VERSION 3.8)

#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17)
set(compiler_flags "-g -DFAKE_JNI_DEBUG")
set(CMAKE_CXX_FLAGS ${compiler_flags})
set(CMAKE_C_FLAGS ${compiler_flags})

project(fake-jni)
add_library(fake-jni INTERFACE)
add_subdirectory(CX)

target_include_directories(fake-jni INTERFACE include src ${cx_SOURCE_DIR})
target_compile_options(fake-jni INTERFACE
        -Wall
        -Wextra
        -pedantic
        -Wno-unused-parameter
        -Wno-unused-function
)

set(BUILD_FAKE_JNI_TESTS ON CACHE BOOL "Enables fake-jni tests")
set(BUILD_FAKE_JNI_EXAMPLES ON CACHE BOOL "Enables fake-jni examples")

if (${BUILD_FAKE_JNI_TESTS})
    add_subdirectory(tests)
endif()

if (${BUILD_FAKE_JNI_EXAMPLES})
    add_subdirectory(examples)
endif()
