cmake_minimum_required(VERSION 3.16)
project(
	ikulab-motion-viwer
	VERSION 0.1.0
	LANGUAGES CXX
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_compile_options(
	-std=c++17 -fdiagnostics-color=always -g
)

if("${BUILD_TYPE}" STREQUAL "Release")
	add_compile_options(-DNODEBUG)
endif()

# external libraries ----------
find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)
add_subdirectory(./includes/imgui)
add_subdirectory(./includes/easylogging++)
add_subdirectory(./includes/vk_mem_alloc)

# shader compile ----------
add_custom_target(
	shaders
	make
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tool
	DEPENDS ./shaders/shader.frag ./shaders/shader.vert
	COMMENT "compiling shaders"
)

# ikulab-motion-viewer app ----------
add_subdirectory(./src/app)
add_dependencies(app shaders)

# app dependencies
target_link_libraries(app easyloggingpp)

# ikura lib ----------
add_subdirectory(./src/lib/ikura)

# ikura dependencies
target_link_libraries(ikura INTERFACE vulkan glfw)

target_link_libraries(renderEngine easyloggingpp vk_mem_alloc)

target_link_libraries(appEngine imgui)

target_link_libraries(nativeWindow easyloggingpp vk_mem_alloc)
target_link_libraries(virtualWindow imgui)

target_link_libraries(renderComponent vk_mem_alloc easyloggingpp)

# link ikura to executable
target_link_libraries(app ikura)