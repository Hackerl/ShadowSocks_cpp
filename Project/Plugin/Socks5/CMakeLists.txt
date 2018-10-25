project(Socks5)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(Socks5 LibMain.cpp Socks5.h Socks5.cpp LibExport.cpp)
    target_link_libraries(Socks5 libSocket.so libPortTunnel.so libEventLoop.so libjsoncpp.a libSocketRelay.so libCommonProxy.so)
else()
    add_library(Socks5 SHARED Socks5.h Socks5.cpp LibExport.cpp)
    target_link_libraries(Socks5)
endif()

