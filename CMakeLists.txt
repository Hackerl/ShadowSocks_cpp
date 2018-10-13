# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.7)

project(ShadowSocksr_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/libevent)
link_directories(${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(Project/Common)
add_subdirectory(Project/Socket)
add_subdirectory(Project/EventLoop)
add_subdirectory(Project/ShadowSocksr)