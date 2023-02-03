# External package
find_package(glfw3 3.3 REQUIRED)

# add libraries
add_library(ikura-appEngine STATIC appEngine.cpp)

file(GLOB RENDER_ENGINE_SOURCE renderEngine/*.cpp)
add_library(ikura-renderEngine STATIC ${RENDER_ENGINE_SOURCE})

target_compile_definitions(ikura-renderEngine PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)

install(FILES
	${PROJECT_SOURCE_DIR}/src/ikura/engine/appEngine.hpp
	DESTINATION include/ikura/engine)

install(FILES
	${PROJECT_SOURCE_DIR}/src/ikura/engine/renderEngine/renderEngine.hpp
	DESTINATION include/ikura/engine/renderEngine)