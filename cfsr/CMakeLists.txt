cmake_minimum_required (VERSION 2.6)

project(cfsr)

# Dirs

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Flags

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -s -static")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -pedantic -Wextra")

# Include headers

include_directories("include")

# Sources

file(GLOB CFSR_SRC "src/*.c")
add_library(cfsr STATIC ${CFSR_SRC})
target_link_libraries(cfsr netcdf.a hdf5_hl.a hdf5.a szip.a z.a curl.a gcc.a) #netcdf_c++4

file(GLOB EXAMPLE_SRC "example/*.cpp")
add_executable(cfsr_example ${EXAMPLE_SRC})
target_link_libraries(cfsr_example cfsr)
