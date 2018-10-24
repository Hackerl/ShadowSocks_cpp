project(Socks5)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(Socks5 LibMain.cpp Socks5.h Socks5.cpp)
else()
    add_library(Socks5 SHARED Socks5.h Socks5.cpp)
endif()

target_link_libraries(Socks5)