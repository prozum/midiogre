add_executable(extract
	extract.c	
)

target_link_libraries(extract
	mid
	analyze
)

install(TARGETS extract DESTINATION bin)
