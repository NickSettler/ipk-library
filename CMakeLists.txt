cmake_minimum_required(VERSION 3.24)
project(ipklib)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

enable_testing()

add_executable(
        tests
        tests/main.cpp
        tests/lexer_tests.cpp)

target_link_libraries(
        tests
        PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

add_executable(ipklib src/main.cpp src/lexer.cpp src/lexer.h src/types.h src/parser.cpp src/parser.h)
