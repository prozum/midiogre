add_executable(exttest
	exttest.c
)

target_link_libraries(exttest
	ext
	mid
)

install(TARGETS exttest DESTINATION bin)
