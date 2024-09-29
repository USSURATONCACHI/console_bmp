enable_testing()

add_executable(run_tests
    src/bit_view.cpp
    src/bmp24.cpp
    src/bmp32.cpp
    src/test_util.cpp
)
target_include_directories(run_tests PRIVATE include)
target_link_libraries(run_tests GTest::gtest_main bmp_reader)

include(GoogleTest)
gtest_discover_tests(run_tests)