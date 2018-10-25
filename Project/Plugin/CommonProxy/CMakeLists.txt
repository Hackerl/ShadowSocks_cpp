project(CommonProxy)

set(CMAKE_CXX_STANDARD 14)

add_library(CommonProxy SHARED CommonProxy.h CommonProxy.cpp LibExport.cpp)

target_link_libraries(CommonProxy)