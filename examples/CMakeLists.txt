file(GLOB example_src "src/*.cpp")
add_executable(examples ${example_src})

target_compile_options(examples PRIVATE -Wall -Wextra -pedantic)
target_include_directories(examples PUBLIC include)
target_link_libraries(examples fake-jni dl)