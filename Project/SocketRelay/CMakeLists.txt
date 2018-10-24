project(SocketRelay)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(SocketRelay LibMain.cpp SocketRelay.h SocketRelay.cpp CommonSocketRelay.cpp CommonSocketRelay.h)
    target_link_libraries(SocketRelay libSocket.so libPortTunnel.so libEventLoop.so libjsoncpp.a libSocks5.so)
else()
    add_library(SocketRelay SHARED SocketRelay.h SocketRelay.cpp CommonSocketRelay.cpp CommonSocketRelay.h LibExport.cpp)
    target_link_libraries(SocketRelay)
endif()