cmake_minimum_required (VERSION 2.6)

project(Sailing)

# Dirs

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Include headers

include_directories("include")

# Sources

file(GLOB CFSR_SRC "src/cfsr.c")
add_library(cfsr ${CFSR_SRC})
target_link_libraries(cfsr netcdf curl) #netcdf_c++4

file(GLOB SAILING_SRC "src/*.cpp")
add_executable(sailing ${SAILING_SRC})
target_link_libraries(sailing cfsr)

# Flags

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
