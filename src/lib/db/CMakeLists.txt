add_library(db
		db.c)

target_link_libraries(db
	mid
	ext
	analyze
	${SQLITE3_LIBRARIES}
	
)

