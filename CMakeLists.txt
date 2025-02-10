cmake_minimum_required(VERSION 3.14)
project(BigInteger VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_GENERATOR)
    if(WIN32)
        set(CMAKE_GENERATOR "Ninja" CACHE INTERNAL "")
    endif()
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MD>
        $<$<CONFIG:Debug>:/MDd>
        $<$<CONFIG:Release>:/MD>
    )
else()
    set(CMAKE_CXX_FLAGS_INIT "-static")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    add_compile_options(-static-libgcc -static-libstdc++)
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_library(biginteger INTERFACE)

target_include_directories(biginteger INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_LIST_DIR}/cmake/cleanup.cmake"
)

add_subdirectory(examples)

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    cmake_policy(SET CMP0135 NEW)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(googletest)
    
    enable_testing()
    add_executable(biginteger_test tests/biginteger_test.cpp)
    if(MSVC)
        target_compile_options(biginteger_test PRIVATE /MDd$<$<CONFIG:Release>:/MD>)
    endif()
    target_link_libraries(biginteger_test PRIVATE
        GTest::gtest_main
        biginteger
    )

    include(GoogleTest)
    gtest_discover_tests(biginteger_test)
endif()
