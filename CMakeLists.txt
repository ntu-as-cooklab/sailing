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
file(GLOB NC_READ_SRC "src/*")
add_executable(nc-read ${NC_READ_SRC})
target_link_libraries(nc-read netcdf) #netcdf_c++4

# Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O")
