cmake_minimum_required(VERSION 3.31)
project(juniperSRC)

set(CMAKE_CXX_STANDARD 20)

add_library(juniperSRC STATIC kernel.cpp
        SimData.cpp
        SimData.h
)
target_include_directories(juniperSRC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
