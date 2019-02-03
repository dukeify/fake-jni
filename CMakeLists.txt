cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

# Enable testing
enable_testing()

# Add JNI headers to `include` path
include_directories(include)

add_subdirectory(jni)
add_subdirectory(example)
add_subdirectory(test)
