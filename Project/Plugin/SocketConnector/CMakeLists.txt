project(SocketConnector)

set(CMAKE_CXX_STANDARD 14)

add_library(SocketConnector SHARED LibExport.cpp SocketConnector.h SocketConnector.cpp)
target_link_libraries(SocketConnector libjsoncpp.so)
