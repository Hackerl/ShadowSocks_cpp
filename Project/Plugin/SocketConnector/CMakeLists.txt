project(SocksConnector)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_DEBUGEXE)
    add_executable(SocksConnector SocketConnector.h SocketConnector.cpp)
    target_link_libraries(SocksConnector)
else()
    add_library(SocksConnector SHARED SocketConnector.h SocketConnector.cpp)
    target_link_libraries(SocksConnector)
endif()