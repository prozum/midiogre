add_executable(dbtest
	dbtest.c
)

target_link_libraries(dbtest
	mid
	${SQLITE3_LIBRARIES}
	
)
if (WIN32)
		target_link_libraries(dbtest
						win
						database
							)
endif()


install(TARGETS dbtest DESTINATION bin)
