include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(indicators_tests
        indicators_tests.cpp)

target_link_libraries(indicators_tests
        PRIVATE
        GTest::gtest_main
        indicators
        core)

add_executable(analysis_tests
        analysis_tests.cpp)

target_link_libraries(analysis_tests
        PRIVATE
        GTest::gtest_main
        analysis
        core)

include(GoogleTest)
gtest_discover_tests(indicators_tests)
gtest_discover_tests(analysis_tests)