project(EventLoop)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(EventLoop EventLoop.cpp EventLoop.h LibExport.cpp LibMain.cpp)
else()
    add_library(EventLoop SHARED EventLoop.cpp EventLoop.h LibExport.cpp ../../include/SocketRelay/ISocketRelay.h ../../include/Node/INode.h)
endif()

target_link_libraries(EventLoop libevent.so)