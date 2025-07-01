cmake_minimum_required(VERSION 3.31)
project(juniperSPH)

set(CMAKE_CXX_STANDARD 20)

add_library(kernelModule kernel.cpp kernel.h)

add_executable(juniperSPH main.cpp)
target_link_libraries(juniperSPH kernelModule)
