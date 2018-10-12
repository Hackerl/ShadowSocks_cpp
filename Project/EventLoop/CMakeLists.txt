project(EventLoop)

set(CMAKE_CXX_STANDARD 14)

add_library(EventLoop SHARED EventLoop.cpp EventLoop.h)

#target_link_libraries(EvenLoop)