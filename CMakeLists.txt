cmake_minimum_required (VERSION 2.6)

project(Sailing)

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
include_directories("float2")
include_directories("cfsr/include")
#include_directories("websocketpp")

# Dependencies

find_package(Boost COMPONENTS system thread REQUIRED)

add_subdirectory(cfsr)

file(GLOB SAILING_SRC "src/*.cpp")
if(WIN32)
    file(GLOB SAILING_RES "res/sailing.res")
    set_source_files_properties(
        ${SAILING_RES} PROPERTIES
        EXTERNAL_OBJECT TRUE  # Identifies this as an object file
        GENERATED TRUE  # Avoids need for file to exist at configure-time
        )
endif()
add_executable(sailing ${SAILING_SRC} ${SAILING_RES})
if(WIN32)
    target_link_libraries(sailing ws2_32 wsock32 cfsr boost_system-mt.a boost_thread-mt.a winpthread.a stdc++.a gcc.a winpthread.a)
else()
    target_link_libraries(sailing cfsr ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} pthread)
endif()
# Install

install(TARGETS sailing DESTINATION ${CMAKE_SOURCE_DIR}/dist)
install(DIRECTORY html/ DESTINATION ${CMAKE_SOURCE_DIR}/dist/html)
install(DIRECTORY data/ DESTINATION ${CMAKE_SOURCE_DIR}/dist/data)
