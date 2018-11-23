cmake_minimum_required(VERSION 3.7)
project(TLSTicketAuth)

set(CMAKE_CXX_STANDARD 14)

add_library(TLSTicketAuth SHARED LibExport.cpp TLSTicketAuth.h TLSTicketAuth.cpp)

target_link_libraries(TLSTicketAuth ${OPENSSL_LIBRARIES})
