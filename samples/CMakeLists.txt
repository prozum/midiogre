add_subdirectory(list-test)
add_subdirectory(ext-test)
add_subdirectory(analyze-test)
add_subdirectory(pop-test)

add_subdirectory(info)

if(SQL)
    add_subdirectory(db-test)
endif()

if(GUI)
    add_subdirectory(ginfo)
endif()
