cmake_minimum_required(VERSION 3.7)
project(HTTPSimple)

set(CMAKE_CXX_STANDARD 14)

add_library(HTTPSimple SHARED LibExport.cpp HTTPSimple.h HTTPSimple.cpp)

target_link_libraries(HTTPSimple)
