cmake_minimum_required(VERSION 3.8)

#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17)

project(fake-jni VERSION 0.1.0)
add_subdirectory(CX)

set(CMAKE_BUILD_TYPE Release)
set(compiler_flags "-g -DFAKE_JNI_DEBUG")
set(CMAKE_CXX_FLAGS ${compiler_flags})
set(CMAKE_C_FLAGS ${compiler_flags})

file(GLOB_RECURSE FAKE_JNI_SRC "src/**.cpp")
add_library(fake-jni SHARED ${FAKE_JNI_SRC})

target_link_libraries(fake-jni dl)
target_include_directories(fake-jni PUBLIC include ${cx_SOURCE_DIR})
target_compile_options(fake-jni PUBLIC
        -Wall
        -Wextra
#        -Werror
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
