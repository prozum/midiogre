add_executable(poptest
        poptest.c
)

target_link_libraries(poptest
        popularity
)

install(TARGETS poptest DESTINATION bin)
