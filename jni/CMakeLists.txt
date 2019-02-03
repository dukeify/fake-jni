# All sources for the JNI target (only STATIC is supported)
add_library(jni STATIC
    fake_jni.cpp
    agent.cpp
)

target_compile_options(jni PRIVATE -Werror -Wall -Wextra -pedantic)
