cmake_minimum_required (VERSION 2.6)

project(cfsr)

# Dirs

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Flags

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -s")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -pedantic -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Ofast -s")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -pedantic -Wextra")

if(WIN32)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
endif()

# Include headers

include_directories("include")

# Sources

file(GLOB CFSR_SRC "src/*.c")
add_library(cfsr STATIC ${CFSR_SRC})
if(WIN32)
    target_link_libraries(cfsr netcdf.a hdf5_hl.a hdf5.a szip.a z.a curl.a winpthread.a stdc++.a gcc.a winpthread.a) #netcdf_c++4
else()
    target_link_libraries(cfsr netcdf curl pthread) #netcdf_c++4
endif()

if(WIN32)
    file(GLOB EXAMPLE_SRC "convert/*.cpp")
    add_executable(convert ${EXAMPLE_SRC})
    target_link_libraries(convert cfsr)
endif()
