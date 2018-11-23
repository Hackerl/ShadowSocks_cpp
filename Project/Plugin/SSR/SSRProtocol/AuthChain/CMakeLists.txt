cmake_minimum_required(VERSION 3.7)
project(AuthChain)

set(CMAKE_CXX_STANDARD 14)

add_library(AuthChain SHARED LibExport.cpp AuthChain.h AuthChain.cpp AuthChainLocal.h AuthChainLocal.cpp)

target_link_libraries(AuthChain libdl.so libpthread.so ${JSONCPP_LIBRARIES} ${OPENSSL_LIBRARIES})