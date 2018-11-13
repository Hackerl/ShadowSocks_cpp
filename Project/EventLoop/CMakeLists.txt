project(EventLoop)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_DEBUGEXE)
    add_executable(EventLoop LibMain.cpp EventLoop.cpp EventLoop.h LibExport.cpp)
else()
    add_library(EventLoop SHARED EventLoop.cpp EventLoop.h LibExport.cpp)
endif()

target_link_libraries(EventLoop libevent.so libglog.so)