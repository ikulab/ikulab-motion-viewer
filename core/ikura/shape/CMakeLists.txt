file(GLOB_RECURSE sources *.cpp)

add_library(ikura-shape STATIC ${sources})

file(GLOB headers ./*.hpp)
install(FILES ${headers}
	DESTINATION include/ikura/shape)

file(GLOB bone_headers bone/*.hpp)
install(FILES ${bone_headers}
	DESTINATION include/ikura/shape/bone)

file(GLOB cube_headers cube/*.hpp)
install(FILES ${cube_headers}
	DESTINATION include/ikura/shape/cube)

file(GLOB debug_headers debug/*.hpp)
install(FILES ${debug_headers}
	DESTINATION include/ikura/shape/debug)

file(GLOB floor_headers floor/*.hpp)
install(FILES ${floor_headers}
	DESTINATION include/ikura/shape/floor)

file(GLOB sphere_headers sphere/*.hpp)
install(FILES ${sphere_headers}
	DESTINATION include/ikura/shape/sphere)
