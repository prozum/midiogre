find_package( PkgConfig REQUIRED )
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories (${GTK3_INCLUDE_DIRS})

link_directories (${CMAKE_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(midiogre_gui
    main.c
)

target_link_libraries( midiogre_gui midi ${GTK3_LIBRARIES})
