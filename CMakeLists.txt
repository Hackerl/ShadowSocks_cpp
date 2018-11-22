# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.7)

project(ShadowSocksr_cpp)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)

pkg_check_modules(JSONCPP REQUIRED jsoncpp)
pkg_check_modules(LIBGLOG REQUIRED libglog)
pkg_check_modules(LIBEVENT REQUIRED libevent)
pkg_check_modules(OPENSSL REQUIRED openssl)

include_directories(${PROJECT_SOURCE_DIR}/include ${JSONCPP_INCLUDE_DIRS})
link_directories(${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(Project/Socket)
add_subdirectory(Project/EventLoop)
add_subdirectory(Project/SocketNode)
add_subdirectory(Project/ShadowSocksr)
add_subdirectory(Project/Plugin)

add_dependencies(ShadowSocksr Socket EventLoop SocketNode)
