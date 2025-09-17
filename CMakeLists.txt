cmake_minimum_required(VERSION 3.16.3)
project(juniperEXE)

# For Profiling Only
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(tests)

add_executable(juniperEXE main.cpp)
target_link_libraries(juniperEXE PUBLIC juniperSRC)
