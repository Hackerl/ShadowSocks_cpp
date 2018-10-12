project(Common)

set(CMAKE_CXX_STANDARD 14)

add_library(Common STATIC JSONHelper.cpp Mutex.cpp)

target_link_libraries(Common libjsoncpp.a)