
cmake_minimum_required(VERSION 3.17)
project(libuv-test C) # libuv-test 为项目名字

set(CMAKE_C_STANDARD 99)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out CACHE STRING "" )
set(LIBUV_DIR lib/libuv)
set(HTTP_PARSER_DIR lib/http-parser)

include_directories(${LIBUV_DIR}/include)
include_directories(${HTTP_PARSER_DIR})

link_directories(${LIBUV_DIR})

add_subdirectory(${LIBUV_DIR} out/lib/libuv)

add_executable(libuv-test test2.c lib/http-parser/http_parser.c)

target_link_libraries(libuv-test uv) # libuv-test 为项目名字