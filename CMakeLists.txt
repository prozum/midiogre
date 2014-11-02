project(midiogre C)

# Version number
set(VERSION_MAJOR   0   CACHE STRING "Major version number.")
set(VERSION_MINOR   0   CACHE STRING "Minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

cmake_minimum_required(VERSION 2.8)

include(cmake/options.cmake)
include(cmake/showoptions.cmake)

include_directories(
        ${CMAKE_SOURCE_DIR}/src/lib
)


add_subdirectory(src)


if (SAMPLES)
	add_subdirectory(samples)
endif()

if (DOC)
	include(cmake/doxygen.cmake)
endif()
