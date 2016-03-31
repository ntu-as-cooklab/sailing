cmake_minimum_required (VERSION 2.6)

project(cfsr)

# Dirs

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Include headers

include_directories("include")

# Sources

file(GLOB CFSR_SRC "src/*.c")
add_library(cfsr ${CFSR_SRC})
if(WIN32)
    target_link_libraries(cfsr netcdf hdf5_hl hdf5 szip z curl) #netcdf_c++4
else()
    target_link_libraries(cfsr netcdf hdf5_hl hdf5 szip z curl) #netcdf_c++4
endif()

# Flags

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -s -static -save-temps")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -pedantic -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -O3 -s -static -save-temps")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -pedantic -Wextra")
