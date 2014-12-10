# Convert resources to c
add_custom_target(GenerateRessources
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  COMMAND ${GLIB_COMPILE_RESOURCES} --generate-source --target=${CMAKE_CURRENT_SOURCE_DIR}/ginfo-res.c ginfo.gresource.xml)

# Compile
add_executable(ginfo
    ginfo-main.c
    ginfo-app.c
    ginfo-window.c
    ginfo-res.c
    ginfo-mid.c)

add_dependencies(ginfo GenerateRessources)

# Link
target_link_libraries(ginfo
                      mid
                      ${GTK3_LIBRARIES})
# Install
install(TARGETS ginfo
        RUNTIME DESTINATION bin)

# TODO: Desktop file
#install (FILES ${CMAKE_SOURCE_DIR}/res/midiogre.desktop
#         DESTINATION share/midiogre)

