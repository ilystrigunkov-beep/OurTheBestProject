cmake_minimum_required(VERSION 3.31)
project(OurProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(OurProject main.cpp
        csr/CSV_edit.cpp
        csr/CSV_edit.h)
