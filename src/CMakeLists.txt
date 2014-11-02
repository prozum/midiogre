include_directories(
        ${CMAKE_SOURCE_DIR}/src/lib
)

add_subdirectory(lib)

if(MAIN)
	find_package( PkgConfig REQUIRED )
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	
	include_directories (${GTK3_INCLUDE_DIRS})
	
	link_directories (${CMAKE_LIBRARY_DIRS})
	
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
