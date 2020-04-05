find_package(GTest REQUIRED)

add_executable(jni-test 
    main.cpp 
    smoke_test.cpp
)

target_include_directories(jni-test PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(jni-test fake-jni ${GTEST_LIBRARIES})

add_test(fake-jni jni-test)