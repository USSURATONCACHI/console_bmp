enable_testing()

add_executable(run_tests
    src/bit_view.cpp
)
target_link_libraries(run_tests GTest::gtest_main bmp_reader)

include(GoogleTest)
gtest_discover_tests(run_tests)