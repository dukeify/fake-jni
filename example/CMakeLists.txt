add_library(example_jni_agent SHARED example_jni_agent.cpp)

add_executable(example example.cpp)
target_link_libraries(example jni dl)
