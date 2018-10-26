project(Socket)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "DebugExe")
    add_executable(Socket LibMain.cpp LibExport.cpp TCPSocket.cpp TCPSocket.h)
    target_link_libraries(Socket libEventLoop.so)
else()
    add_library(Socket SHARED LibExport.cpp TCPSocket.cpp TCPSocket.h)
    target_link_libraries(Socket)
endif()

