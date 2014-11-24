add_executable(crittest
	crittest.c
)

target_link_libraries(crittest
	mid
)

install(TARGETS crittest DESTINATION bin)
