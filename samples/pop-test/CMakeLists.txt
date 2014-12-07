add_executable(pop-test
        pop-test.c
)

target_link_libraries(pop-test
        pop
)

install(TARGETS pop-test DESTINATION bin)
