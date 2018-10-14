project(TCPRelay)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(TCPRelay LibMain.cpp)
else()
    add_library(TCPRelay SHARED TCPRelay.h TCPRelay.cpp)
endif()

target_link_libraries(TCPRelay)