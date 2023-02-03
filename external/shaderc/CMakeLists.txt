file(GLOB headers "*.h" "*.hpp")

install(FILES ${headers}
	DESTINATION include/ikura/shaderc)