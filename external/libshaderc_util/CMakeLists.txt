file(GLOB headers "*.h" "*.hpp" "*.inc")

install(FILES ${headers}
	DESTINATION include/ikura/libshaderc_util)