cmake_minimum_required(VERSION 2.8)

include(cmake/options.cmake)

include(cmake/showoptions.cmake)

project(midiogre C)

if (DOC)
    find_package(Doxygen)

    if(NOT DOXYGEN_FOUND)
        	message(FATAL_ERROR "Doxygen not found!")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM)

	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
endif()

add_subdirectory(src)

