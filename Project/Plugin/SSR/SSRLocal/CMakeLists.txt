cmake_minimum_required(VERSION 3.7)
project(SSRLocal)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_DEBUGEXE)
    add_executable(SSRLocal LibMain.cpp SSRLocal.h SSRLocal.cpp)
else()
    add_library(SSRLocal SHARED LibExport.cpp SSRLocal.h SSRLocal.cpp)
endif()

target_link_libraries(SSRLocal libSSRObfs.a libSSRProtocol.a libdl.so libpthread.so ${JSONCPP_LIBRARIES} ${OPENSSL_LIBRARIES})