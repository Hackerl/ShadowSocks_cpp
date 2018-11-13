project(SocketNode)

set(CMAKE_CXX_STANDARD 14)

add_library(SocketNode SHARED SocketNode.h SocketNode.cpp LocalSocketNode.h RemoteSocketNode.h LibExport.cpp)
target_link_libraries(SocketNode libglog.so)