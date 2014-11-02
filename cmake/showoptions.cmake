message("* Installing to : ${CMAKE_INSTALL_PREFIX}")
message("")

if( MAIN )
    message("* Build Midiogre : Yes")
else()
    message("* Build Midiogre : No (default)")
endif()

if( MINGW )
    message("* Build with MinGW : Yes")
else()
    message("* Build with MinGW : No (default)")
endif()

if( DOC )
    message("* Generate documentaion : Yes ")
else()
    message("* Generate documentaion : No (default)")
endif()

if( SAMPLES )
	message("* Build samples : Yes (default)")
else()
	message("* Build samples : No ")
endif()
