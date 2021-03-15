
cmake_minimum_required(VERSION 3.17)
project(libuv-cmake C) # libuv-cmake 为项目名字

set(CMAKE_C_STANDARD 99)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
set(LIBUV_DIR libuv)

include_directories(${LIBUV_DIR}/include)

link_directories(${LIBUV_DIR})

add_subdirectory(${LIBUV_DIR} out/libuv)

add_executable(libuv-cmake test1.c)

find_library(uv out)

target_link_libraries(libuv-cmake uv) # libuv-cmake 为项目名字

