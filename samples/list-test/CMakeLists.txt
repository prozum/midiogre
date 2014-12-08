add_executable(list-test
	list-test.c
)

target_link_libraries(list-test
	list
)

install(TARGETS list-test DESTINATION bin)
