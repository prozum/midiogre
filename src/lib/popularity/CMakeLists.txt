add_library(popularity
    popularity.c
    pop_balance.c
)

target_link_libraries(popularity
                      m)
