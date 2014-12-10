add_executable(db-test
	db-test.c
)

target_link_libraries(db-test
	mid
	db
	${SQLITE3_LIBRARIES}
	${GLIB2_LIBRARIES}

)
if (WIN32)
		target_link_libraries(db-test
					win)
endif()


install(TARGETS db-test DESTINATION bin)
