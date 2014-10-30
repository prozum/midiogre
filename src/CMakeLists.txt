add_subdirectory(midi)

include_directories(
	${CMAKE_SOURCE_DIR}/src
)

if(GUI)
	add_subdirectory(gui)
endif(GUI)

if(CLI)
    add_subdirectory(cli)
endif(CLI)
