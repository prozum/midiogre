pkg_check_modules_fullpath(SQLITE3 sqlite3)
if(NOT SQLITE3_FOUND)
    message(FATAL_ERROR  "Please install: sqlite-devel")
endif()

include_directories(${SQLITE3_INCLUDE_DIRS})
add_definitions(${SQLITE3_CFLAGS_OTHER})

add_executable(db
	db.c
)

target_link_libraries(db
	mid
	${SQLITE3_LIBRARIES}
	
)

install(TARGETS db DESTINATION bin)
