add_executable(analyze-test
	analyze-test.c	
)

target_link_libraries(analyze-test
	mid
	analyze
)

install(TARGETS analyze-test DESTINATION bin)
