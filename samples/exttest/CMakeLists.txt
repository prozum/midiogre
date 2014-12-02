add_executable(exttest
	crittest.c
)

target_link_libraries(exttest
	ext
	mid
)

install(TARGETS exttest DESTINATION bin)
