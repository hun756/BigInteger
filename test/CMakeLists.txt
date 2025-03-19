include(${CMAKE_SOURCE_DIR}/cmake/FindGTest.cmake)
include(GoogleTest)

set(TEST_SOURCES
    biginteger_test.cpp
    hex_conversion_test.cpp
    bit_manipulation_test.cpp
    arithmetic_operations_test.cpp
)

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name}
        PRIVATE
        GTest::gtest
        GTest::gtest_main
        ${PROJECT_NAME}
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()