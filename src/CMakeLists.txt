# Add libs dir
add_subdirectory(lib)

# Main Program 
if(MAIN)

    # Setup ressource generation
    add_custom_target(GenerateRessources
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/res/
                  COMMAND ${GLIB_COMPILE_RESOURCES} --generate-source --target=${CMAKE_CURRENT_SOURCE_DIR}/midiogre-res.c midiogre.gresource.xml)
    
    # Compile
    add_executable(midiogre
        midiogre-main.c
        midiogre-res.c
        midiogre-gui.c
        midiogre-import.c
        midiogre-song.c)

    # Generate ressources before build
    add_dependencies(midiogre GenerateRessources)

    # Link
    target_link_libraries(midiogre
                          mid
                          db
                          pop
                          ${GTK3_LIBRARIES})
    
    # Install
    install(TARGETS midiogre
            RUNTIME DESTINATION bin)

    # TODO: Desktop file
    #install (FILES ${CMAKE_SOURCE_DIR}/res/midiogre.desktop
    #         DESTINATION share/midiogre)

endif(MAIN)
