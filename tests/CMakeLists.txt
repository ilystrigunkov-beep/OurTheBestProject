set(gtest_build_samples OFF CACHE BOOL "" FORCE)
set(gtest_build_tests OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
set(gtest_disable_pthreads OFF CACHE BOOL "" FORCE)
set(gtest_hide_internal_symbols OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

set(GOOGLETEST_VERSION 1.15.2 CACHE INTERNAL "")

set(GTEST_BINARY_DIR ${CMAKE_BINARY_DIR}/googletest)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/googletest ${GTEST_BINARY_DIR} EXCLUDE_FROM_ALL)

include(GoogleTest)

set(TEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

function(add_project_test target_name source_file)
  add_executable(${target_name}
    ${CMAKE_CURRENT_SOURCE_DIR}/${source_file})
  target_link_libraries(${target_name}
    PRIVATE
      GTest::gtest_main
      ${ARGN})
  target_include_directories(${target_name}
    PRIVATE
      ${TEST_INCLUDE_DIR})
  gtest_discover_tests(${target_name})
endfunction()

add_project_test(core_tests core_tests.cpp core)
add_project_test(indicators_tests indicators_tests.cpp indicators)
add_project_test(analysis_tests analysis_tests.cpp analysis)
