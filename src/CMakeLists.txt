# Add libs dir
add_subdirectory(lib)

# Main Program 
if(MAIN)
    
    # Compile
    add_executable(midiogre
        midiogre.c)

    # Link
    target_link_libraries(midiogre
                          mid
                          ${GTK3_LIBRARIES})
    # Install
    install(TARGETS midiogre
            RUNTIME DESTINATION bin)

    # TODO: Desktop file
    #install (FILES ${CMAKE_SOURCE_DIR}/res/midiogre.desktop
    #         DESTINATION share/midiogre)

endif(MAIN)
