project(SocksClient)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_DEBUGEXE)
    add_executable(SocksClient LibMain.cpp LibExport.cpp SocksClient.h SocksClient.cpp)
    target_link_libraries(SocksClient libSocket.so libEventLoop.so libjsoncpp.a libSocketNode.so libSocksServer.so)
else()
    add_library(SocksClient SHARED LibExport.cpp SocksClient.h SocksClient.cpp)
    target_link_libraries(SocksClient)
endif()

