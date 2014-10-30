add_subdirectory(midi)

include_directories(
	${CMAKE_SOURCE_DIR}/src
)

if(GUI)
	add_subdirectory(gui)
else()
	add_subdirectory(cli)
endif(GUI)

