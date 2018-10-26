project(ProxyServer)

set(CMAKE_CXX_STANDARD 14)

add_library(ProxyServer SHARED LibExport.cpp ProxyServer.h ProxyServer.cpp)

target_link_libraries(ProxyServer)