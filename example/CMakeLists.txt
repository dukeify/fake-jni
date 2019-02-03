set(CMAKE_CXX_STANDARD 17)

add_library(example_jni_agent SHARED example_jni_agent.cpp)

add_executable(example example.cpp)
add_executable(example_objects example_object.cpp)

target_link_libraries(example_objects jni dl)
target_link_libraries(example jni dl)
