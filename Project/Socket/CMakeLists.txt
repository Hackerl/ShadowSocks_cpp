project(Socket)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(Socket LibMain.cpp LibExport.cpp TCPSocket.cpp TCPSocket.h SocketPipe.h SocketPipe.cpp)
else()
    add_library(Socket SHARED LibExport.cpp TCPSocket.cpp TCPSocket.h SocketPipe.h SocketPipe.cpp)
endif()

target_link_libraries(Socket libEventLoop.so)