add_library(pop
    pop.c
)

target_link_libraries(pop
                      analyze
                      list
                      m)
