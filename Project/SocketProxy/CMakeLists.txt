project(SocketProxy)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_DEBUGEXE)
    add_executable(SocketProxy LibMain.cpp HTTPTunnel.h HTTPTunnel.cpp)
    target_link_libraries(SocketProxy libSocket.so)
else()
    add_library(SocketProxy SHARED LibExport.cpp HTTPTunnel.h HTTPTunnel.cpp)
    target_link_libraries(SocketProxy)
endif()
