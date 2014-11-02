add_executable(info
	info.c
)

target_link_libraries(info
	mid
)

install(TARGETS info DESTINATION bin)
