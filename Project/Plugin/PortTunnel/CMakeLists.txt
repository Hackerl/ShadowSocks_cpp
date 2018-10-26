project(PortTunnel)

set(CMAKE_CXX_STANDARD 14)

add_library(PortTunnel SHARED PortTunnel.h LibExport.cpp)

target_link_libraries(PortTunnel libjsoncpp.a)