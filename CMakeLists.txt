cmake_minimum_required(VERSION 3.20)
project(OurProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

add_subdirectory(src)
add_subdirectory(app)
if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
