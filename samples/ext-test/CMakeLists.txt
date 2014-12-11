add_executable(ext-test
	ext-test.c
)

target_link_libraries(ext-test
	ext
	mid
)

install(TARGETS ext-test DESTINATION bin)
