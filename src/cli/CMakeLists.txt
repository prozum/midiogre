add_executable(midiogre_cli
	main.c
)

target_link_libraries(midiogre_cli
	midi
)

install(TARGETS midiogre_cli DESTINATION bin)
