cmake_minimum_required(VERSION 3.8)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)

project(fake-jni VERSION 0.1.0)
add_subdirectory(CX)

set(BUILD_FAKE_JNI_DEBUG OFF CACHE BOOL "Enables fake-jni debug building")
if(${BUILD_FAKE_JNI_DEBUG})
    set(CMAKE_BUILD_TYPE Release)
    set(compiler_flags "\
        -g\
        -fstandalone-debug\
        -O0\
        -fno-inline\
        -DFAKE_JNI_DEBUG\
    ")
else()
    set(CMAKE_BUILD_TYPE Debug)
    set(compiler_flags "-O3")
endif()

if(${BUILD_FAKE_JNI_ASAN})
    set(compiler_flgs ${compiler_flags} -fsanitize=address)
endif()

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${compiler_flags})
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${compiler_flags})

if(NOT ${FFI_CC})
    set(FFI_CC ${CMAKE_C_COMPILER})
endif()

if(NOT ${FFI_CXX})
    set(FFI_CXX ${CMAKE_CXX_COMPILER})
endif()

ExternalProject_Add(ffi
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/libffi
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh && <SOURCE_DIR>/configure --disable-shared --disable-builddir --prefix=<INSTALL_DIR> --host=${CMAKE_CXX_COMPILER_TARGET} CC=${FFI_CC} CXX=${FFI_CXX}
    BUILD_IN_SOURCE 1
    BUILD_BYPRODUCTS <INSTALL_DIR>/lib/libffi.a
)

ExternalProject_Get_Property(ffi INSTALL_DIR)
set(LIBFFI_INSTALL_DIR ${INSTALL_DIR}/lib/libffi-3.2.1/)
ExternalProject_Get_Property(ffi SOURCE_DIR)
set(ffi_SOURCE_DIR ${SOURCE_DIR})

add_library(libffi STATIC IMPORTED GLOBAL)
file(MAKE_DIRECTORY ${LIBFFI_INSTALL_DIR}/include)
set_target_properties(libffi PROPERTIES
    IMPORTED_LOCATION ${INSTALL_DIR}/lib/libffi.a
    INTERFACE_INCLUDE_DIRECTORIES ${LIBFFI_INSTALL_DIR}/include
)

file(GLOB_RECURSE FAKE_JNI_SRC "src/**.cpp")
add_library(fake-jni SHARED ${FAKE_JNI_SRC})

target_link_libraries(fake-jni dl libffi)
add_dependencies(fake-jni ffi)
target_include_directories(fake-jni PUBLIC
    include
    ${cx_SOURCE_DIR}
    ${ffi_SOURCE_DIR}/include
)
target_compile_options(fake-jni PUBLIC
    -Wall
    -Wextra
#    -Werror
    -pedantic
    -Wno-unused-parameter
    -Wno-unused-function
    -Wno-gnu-anonymous-struct
    -Wno-nested-anon-types
    -Wno-vla-extension
)

set(BUILD_FAKE_JNI_TESTS ON CACHE BOOL "Enables fake-jni tests")
set(BUILD_FAKE_JNI_EXAMPLES ON CACHE BOOL "Enables fake-jni examples")

if (${BUILD_FAKE_JNI_TESTS})
    add_subdirectory(tests)
endif()

if (${BUILD_FAKE_JNI_EXAMPLES})
    add_subdirectory(examples)
endif()
