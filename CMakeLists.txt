cmake_minimum_required(VERSION 3.10)
project(sem9_hanoi)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include)
add_executable(hanoi
    src/main.cpp
    src/hanoi.cpp
)
