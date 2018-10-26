project(SocksConnector)

set(CMAKE_CXX_STANDARD 14)

add_library(SocksConnector SHARED SocketConnector.h SocketConnector.cpp)
target_link_libraries(SocksConnector)
