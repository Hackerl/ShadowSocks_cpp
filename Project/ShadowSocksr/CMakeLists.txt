project(ShadowSocksr)

set(CMAKE_CXX_STANDARD 14)

add_executable(ShadowSocksr main.cpp ShadowSocks.cpp ShadowSocks.h)

target_link_libraries(ShadowSocksr libjsoncpp.so libglog.so libEventLoop.so libSocket.so libSocketNode.so libdl.so)