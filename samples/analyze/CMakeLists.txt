add_executable(analyze
	distance-wip.c	
)

target_link_libraries(analyze
	midi
)

install(TARGETS analyze DESTINATION bin)
