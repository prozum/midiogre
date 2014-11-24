add_executable(dbtest
	dbtest.c
)

target_link_libraries(dbtest
	mid
	${SQLITE3_LIBRARIES}
	
)

install(TARGETS dbtest DESTINATION bin)
