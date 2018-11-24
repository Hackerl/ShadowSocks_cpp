project(Plugins)

set(CMAKE_CXX_STANDARD 14)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/plugin)

add_subdirectory(PortTunnel)
add_subdirectory(SocksClient)
add_subdirectory(SocksServer)
add_subdirectory(SocketConnector)
add_subdirectory(SSR)