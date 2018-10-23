project(PortTunnel)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(PortTunnel LibMain.cpp  PortTunnel.h)
else()
    add_library(PortTunnel SHARED LibMain.cpp PortTunnel.h LibExport.cpp)
endif()

target_link_libraries(PortTunnel libjsoncpp.a libSocket.so)