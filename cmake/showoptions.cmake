message("* Installing to : ${CMAKE_INSTALL_PREFIX}")
message("")

if( GUI )
    message("* Build GTK GUI : Yes")
else()
    message("* Build GTK GUI : No (default)")
endif()

if( MINGW )
    message("* Build with MinGW : Yes")
else()
    message("* Build with MinGW : No (default)")
endif()


if( TESTS )
	message("* Build tests : Yes ")
else()
	message("* Build tests : No (default)")
endif()
