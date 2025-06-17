include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE TEST_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/inerpreter_tests/*.cpp"
)

add_executable(
  itmoscript_tests
  # loop_and_branch_test.cpp
  # files_tests.cpp
  # inerpreter_tests/interpreter_tests.cpp
  ${TEST_SOURCES}
  # inerpreter_tests/non_wild_tests.cpp
  # inerpreter_tests/strings_tests.cpp
  # illegal_ops_test.cpp
)

target_link_libraries(
  itmoscript_tests
  itmoscript
  GTest::gtest_main
  GTest::gmock_main
)

target_include_directories(itmoscript_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(itmoscript_tests)