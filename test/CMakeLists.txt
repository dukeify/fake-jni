include(ExternalProject)
ExternalProject_Add(googletest
  GIT_REPOSITORY    https://github.com/google/googletest.git
  GIT_TAG           master
  INSTALL_COMMAND   ""
)

add_executable(example_test_1 example_test_1.cpp)
target_link_libraries(example_test_1 gtest gtest_main)

add_test(example_test_1 example_test_1)
