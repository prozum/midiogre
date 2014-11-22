add_executable(poptest
        poptest.c
)

target_link_libraries(poptest
        pop
)

install(TARGETS poptest DESTINATION bin)
