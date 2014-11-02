add_executable(info
	main.c
)

target_link_libraries(info
	midi
)

install(TARGETS info DESTINATION bin)
