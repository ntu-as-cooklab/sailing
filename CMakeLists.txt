cmake_minimum_required (VERSION 2.6)

project(Sailing)

# Dirs

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Include headers

include_directories("include")
include_directories("float2")
include_directories("cfsr/include")
include_directories("http_server/include")
include_directories("websocketpp")

# Dependencies

add_subdirectory(cfsr)
add_subdirectory(http_server)

# Sources

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
target_link_libraries(sailing cfsr http_server)

# Flags

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -s -static -save-temps")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -pedantic -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -O3 -s -static -save-temps")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -pedantic -Wextra")
