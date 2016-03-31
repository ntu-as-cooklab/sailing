cmake_minimum_required (VERSION 2.6)

project(vec2)

# Dirs

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Include headers

include_directories(".")

# Sources

file(GLOB SRC "./*.cpp")
add_executable(vec2 ${SRC})

# Flags

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -O3 -s -static -save-temps")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -pedantic -Wextra")
