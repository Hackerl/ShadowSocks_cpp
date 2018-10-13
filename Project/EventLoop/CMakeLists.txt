project(EventLoop)

set(CMAKE_CXX_STANDARD 14)

add_library(EventLoop SHARED EventLoop.cpp EventLoop.h LibExport.cpp)

target_link_libraries(EventLoop libevent.so)