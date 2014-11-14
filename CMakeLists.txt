project(midiogre C)

# Version number
set(VERSION_MAJOR   0   CACHE STRING "Major version number.")
set(VERSION_MINOR   0   CACHE STRING "Minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

cmake_minimum_required(VERSION 2.8)

include(cmake/options.cmake)
include(cmake/showoptions.cmake)
include(cmake/PkgCheckModulesFullpath.cmake)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/lib
)

if (DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -lm")
else()
    set(CMAKE_BUILD_TYPE Release)
endif(DEBUG)


add_subdirectory(src)


if (SAMPLES)
	add_subdirectory(samples)
endif()

if (DOC)
	include(cmake/doxygen.cmake)
endif()
