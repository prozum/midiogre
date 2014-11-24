add_subdirectory(crittest)
add_subdirectory(info)
add_subdirectory(extract)
add_subdirectory(poptest)

if(SQL)
    add_subdirectory(dbtest)
endif()

if(GUI)
    add_subdirectory(ginfo)
endif()
