project(TCPRelay)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(TCPRelay LibMain.cpp SocketRelay.h SocketRelay.cpp CommonSocketRelay.cpp CommonSocketRelay.h)
    target_link_libraries(TCPRelay libSocket.so libPortTunnel.so libEventLoop.so libjsoncpp.a)
else()
    add_library(TCPRelay SHARED SocketRelay.h SocketRelay.cpp CommonSocketRelay.cpp CommonSocketRelay.h)
    target_link_libraries(TCPRelay)
endif()