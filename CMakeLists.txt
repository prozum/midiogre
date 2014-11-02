project(midiogre C)

cmake_minimum_required(VERSION 2.8)

include(cmake/options.cmake)

include(cmake/showoptions.cmake)

include(cmake/doxygen.cmake)

add_subdirectory(src)

if (SAMPLES)
	add_subdirectory(samples)
endif()
