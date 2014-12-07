add_library(db
		db.c)

target_link_libraries(db
	mid
	${SQLITE3_LIBRARIES}
	
)
if (WIN32)
		target_link_libraries(db
				      win)
endif()

