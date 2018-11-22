cmake_minimum_required(VERSION 3.7)
project(SSRProtocol)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_DEBUGEXE)
    add_executable(SSRProtocol LibMain.cpp AuthChain.h AuthChain.cpp AuthChainLocal.h AuthChainLocal.cpp)
else()
    add_library(SSRProtocol STATIC AuthChain.h AuthChain.cpp AuthChainLocal.h AuthChainLocal.cpp)
endif()

target_link_libraries(SSRProtocol libdl.so libpthread.so libssl.so libcrypto.so)