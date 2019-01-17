project(PortTunnel)

set(CMAKE_CXX_STANDARD 14)

add_library(PortTunnel SHARED PortTunnel.h LibExport.cpp PortTunnel.cpp)

target_link_libraries(PortTunnel ${JSONCPP_LIBRARIES})