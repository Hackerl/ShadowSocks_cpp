project(ShadowSocksr)

set(CMAKE_CXX_STANDARD 14)

add_executable(ShadowSocksr main.cpp)

target_link_libraries(ShadowSocksr libCommon.a libjsoncpp.a)