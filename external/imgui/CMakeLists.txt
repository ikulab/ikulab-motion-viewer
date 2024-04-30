file(GLOB sources *.cpp)

set(ikura-ext-src
	${ikura-ext-src} ${sources}
	PARENT_SCOPE)

add_library(imgui STATIC ${sources})

file(GLOB headers *.h)
install(FILES
	${headers}
	DESTINATION include/ikura/imgui)

target_link_libraries(imgui PRIVATE Vulkan::Vulkan)