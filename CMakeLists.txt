cmake_minimum_required(VERSION 3.31)
project(OurProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(OurProject
        app/main.cpp
        csr/CSV/csv_loader.cpp
        csr/CSV/csv_loader.h
        csr/CSV/csv_loader.cpp)

add_subdirectory(app)
add_subdirectory(src)
