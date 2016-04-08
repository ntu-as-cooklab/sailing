cmake_minimum_required (VERSION 2.6)

project(http_server)

# Dirs

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Flags

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -s -static ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -pedantic -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Ofast -s -static -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -pedantic -Wextra")

# Include headers

include_directories("include")

# Dependencies

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system thread REQUIRED)

# Sources

file(GLOB HTTP_SERVER_SRC "src/*.cpp")
add_library(http_server ${HTTP_SERVER_SRC})
target_link_libraries(http_server ws2_32 wsock32 ${Boost_LIBRARIES})
