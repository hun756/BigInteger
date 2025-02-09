add_executable(biginteger_test
    biginteger_test.cpp
)

target_link_libraries(biginteger_test
    PRIVATE
    biginteger
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(biginteger_test)
