project(SocketNode)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(SocketNode SocketNode.h SocketNode.cpp LocalSocketNode.h RemoteSocketNode.h)
    target_link_libraries(SocketRelay)
else()
    add_library(SocketNode SHARED SocketNode.h SocketNode.cpp LocalSocketNode.h RemoteSocketNode.h)
    target_link_libraries(SocketNode)
endif()