file(GLOB_RECURSE EXAMPLE_SRC "src/**.cpp")
add_executable(examples ${EXAMPLE_SRC})

target_compile_options(examples PRIVATE
        -Wall
        -Wextra
        -pedantic
#        -masm=intel
#        -S
#        -Xclang
#        -ast-print
)
target_include_directories(examples PUBLIC include)
target_link_libraries(examples fake-jni dl)
