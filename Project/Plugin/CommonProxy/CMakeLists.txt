project(CommonProxy)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(CommonProxy CommonProxy.h CommonProxy.cpp)
else()
    add_library(CommonProxy SHARED CommonProxy.h CommonProxy.cpp)
endif()

target_link_libraries(CommonProxy)