# Add libs dir
add_subdirectory(lib)

# Main Program 
if(MAIN)
    # Find GTK3
    pkg_check_modules_fullpath(GTK3 gtk+-3.0>=3.10)
    if (NOT GTK3_FOUND)
        message(FATAL_ERROR  "Please install: gtk3-devel")
    endif()
    
    include_directories(${GTK3_INCLUDE_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER})

    # Find command: glib-compile-resources
    _pkgconfig_invoke("glib-2.0" GLIB2 PREFIX
                      "" "--variable=prefix")

    find_program(GLIB_COMPILE_RESOURCES
                NAMES glib-compile-resources
                HINTS ${GLIB2_PREFIX})

    if (NOT GLIB_COMPILE_RESOURCES)
        message(FATAL_ERROR "Please install: glib2-devel")
    endif()

    # Convert resources to c
    add_custom_target(GenerateRessources
                      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/res/
                      COMMAND ${GLIB_COMPILE_RESOURCES} --generate-source --target=${CMAKE_CURRENT_SOURCE_DIR}/midiogre-res.c midiogre.gresource.xml)

    # Compile
    add_executable(midiogre
        midiogre-main.c
        midiogre-app.c
        midiogre-window.c
        midiogre-res.c
        midiogre-mid.c)

    add_dependencies(midiogre GenerateRessources)
    # Link
    target_link_libraries(midiogre
                          mid
                          ${GTK3_LIBRARIES})
    # Install
    install(TARGETS midiogre
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib
            COMPONENT library)

    # TODO: Desktop file
    #install (FILES ${CMAKE_SOURCE_DIR}/res/midiogre.desktop
    #         DESTINATION share/midiogre)

endif(MAIN)
