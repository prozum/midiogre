add_executable(ginfo
	ginfo.c)

target_link_libraries(ginfo
	mid
    ${GTK3_LIBRARIES})

file(COPY ginfo.ui DESTINATION ${CMAKE_BINARY_DIR}/samples/ginfo/)
install(TARGETS ginfo DESTINATION bin)
