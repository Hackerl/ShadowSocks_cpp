project(SSR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(${CMAKE_SOURCE_DIR}/include/Plugin/SSR)
link_directories(${CMAKE_SOURCE_DIR}/bin/plugin)

add_subdirectory(SSRObfs)
add_subdirectory(SSRProtocol)
add_subdirectory(SSRLocal)

add_dependencies(SSRLocal SSRObfs SSRProtocol)