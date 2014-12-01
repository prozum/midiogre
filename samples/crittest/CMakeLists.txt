add_executable(crittest
	crittest.c
)

target_link_libraries(crittest
	ext
	mid
)

install(TARGETS crittest DESTINATION bin)
