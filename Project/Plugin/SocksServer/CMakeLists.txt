project(SocksServer)

set(CMAKE_CXX_STANDARD 14)

add_library(SocksServer SHARED LibExport.cpp SocksServer.h SocksServer.cpp)

target_link_libraries(SocksServer libjsoncpp.so)