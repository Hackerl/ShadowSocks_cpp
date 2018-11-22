cmake_minimum_required(VERSION 3.7)
project(SSRObfs)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_DEBUGEXE)
    add_executable(SSRObfs LibMain.cpp HTTPSimple.h HTTPSimple.cpp TLSTicketAuth.h TLSTicketAuth.cpp)
    target_link_libraries(SSRObfs libdl.so libpthread.so libssl.so libcrypto.so)
else()
    add_library(SSRObfs STATIC HTTPSimple.h HTTPSimple.cpp TLSTicketAuth.h TLSTicketAuth.cpp)
    target_link_libraries(SSRObfs)
endif()