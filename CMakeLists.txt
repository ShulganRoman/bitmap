cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bitmap main.cpp bmp.h defs.h color.h bm_header.h)

target_link_libraries(bitmap PRIVATE "-framework CoreGraphics")
