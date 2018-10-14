project(Pipe)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(Pipe LibMain.cpp Pipe.cpp)
else()
    add_library(Pipe STATIC Pipe.cpp)
endif()

target_link_libraries(Pipe)