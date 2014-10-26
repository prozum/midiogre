cmake_minimum_required(VERSION 2.8)

include(cmake/options.cmake)

include(cmake/showoptions.cmake)

if (MINGW)
endif(MINGW)

project(midiogre C)


add_subdirectory(src)

