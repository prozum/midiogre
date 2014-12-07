add_library(database
		db.c
)
target_link_libraries(database
	mid
	${SQLITE3_LIBRARIES}
	
)
if (WIN32)
		target_link_libraries(database
						win
							)
endif()

