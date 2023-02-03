file(GLOB_RECURSE sources *.cpp)

add_library(ikura-renderComponent STATIC ${sources})

file(GLOB headers ./*.hpp)
install(FILES ${headers}
	DESTINATION include/ikura/renderComponent)

file(GLOB basic_headers basic/*.hpp)
install(FILES ${basic_headers}
	DESTINATION include/ikura/renderComponent/basic)