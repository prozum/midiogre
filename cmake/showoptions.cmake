message("* Installing to : ${CMAKE_INSTALL_PREFIX}")
message("")

if( GUI )
	message("* Build GTK gui : Yes")
else()
	message("* Build GTK gui : No (default)")
endif()

if( TESTS )
	message("* Build tests : Yes ")
else()
	message("* Build tests : No (default)")
endif()
