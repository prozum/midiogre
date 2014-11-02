add_subdirectory(lib)

if(MAIN)
    find_package( PkgConfig )
    if (NOT PKGCONFIG_FOUND)
        message(FATAL_ERROR  "Please install: pkgconfig")
    endif()

    pkg_check_modules(GTK3 gtk+-3.0)
    if (NOT GTK3_FOUND)
        message(FATAL_ERROR  "Please install: gtk3-devel")
    endif()

    include_directories(${GTK3_INCLUDE_DIRS})

    link_directories(${CMAKE_LIBRARY_DIRS})

    add_definitions(${GTK3_CFLAGS_OTHER})

    add_executable(midiogre
        midiogre-main.c
        midiogre-app.c
        midiogre-window.c
        midiogre-mid.c
    )

    target_link_libraries( midiogre mid ${GTK3_LIBRARIES})

    install(TARGETS midiogre
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION lib
            LIBRARY DESTINATION lib
            COMPONENT library)

endif(MAIN)
