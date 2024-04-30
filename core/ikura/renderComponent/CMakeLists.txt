file(GLOB_RECURSE sources *.cpp)

add_library(ikura-renderComponent STATIC ${sources})

target_link_libraries(ikura-renderComponent ikura-common)

target_link_libraries(ikura-renderComponent PkgConfig::easyloggingpp)

file(GLOB headers ./*.hpp)
install(FILES ${headers}
	DESTINATION include/ikura/renderComponent)

file(GLOB basic_headers basic/*.hpp)
install(FILES ${basic_headers}
	DESTINATION include/ikura/renderComponent/basic)