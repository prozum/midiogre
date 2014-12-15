project(midiogre C)

# Version number
set(VERSION_MAJOR   0   CACHE STRING "Major version number.")
set(VERSION_MINOR   0   CACHE STRING "Minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

cmake_minimum_required(VERSION 2.8)

# CMake tools
include(cmake/options.cmake)
include(cmake/showoptions.cmake)
include(cmake/PkgCheckModulesFullpath.cmake)
include(cmake/deps.cmake)

if(DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wno-unused-parameter")
else()
    set(CMAKE_BUILD_TYPE Release)
    if(WIN32)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows")
    endif()
endif()

# Include libs
include_directories(
    src/lib
)

# Main source dir
add_subdirectory(src)

# Samples source dir
if(SAMPLES)
	add_subdirectory(samples)
endif()

# Generate documentation
if(DOC)
	include(cmake/doxygen.cmake)
endif()
