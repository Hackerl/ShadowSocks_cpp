cmake_minimum_required(VERSION 3.7)
project(SSRProtocol)

set(CMAKE_CXX_STANDARD 14)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/plugin/SSR)

add_subdirectory(AuthChain)
add_subdirectory(TLSTicketAuth)
add_subdirectory(HTTPSimple)