file(GLOB test_src "src/*.cpp")
add_executable(tests ${test_src})

target_compile_options(tests PRIVATE -Wall -Wextra -pedantic)
target_include_directories(tests PUBLIC include)
target_link_libraries(tests fake-jni dl)