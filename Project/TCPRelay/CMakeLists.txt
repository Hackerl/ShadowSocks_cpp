project(TCPRelay)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(TCPRelay LibMain.cpp TCPRelay.h TCPRelay.cpp)
    target_link_libraries(TCPRelay libSocket.so libPortTunnel.so libEventLoop.so libjsoncpp.a)
else()
    add_library(TCPRelay SHARED TCPRelay.h TCPRelay.cpp)
    target_link_libraries(TCPRelay)
endif()