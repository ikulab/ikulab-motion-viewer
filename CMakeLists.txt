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

if(${BUILD_TYPE} STREQUAL "Release")
	add_compile_options(-DNODEBUG)
endif()

# external libraries ----------
find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)
add_subdirectory(./includes/imgui)

add_compile_options(-DLOG_USE_COLOR)
add_subdirectory(./includes/logc)

# shader compile ----------
add_custom_target(
	shaders
	make
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tool
	DEPENDS ./shaders/shader.frag ./shaders/shader.vert
	COMMENT "compiling shaders"
)

# executable ----------
add_executable(app ./src/app/main.cpp ./src/app/app.cpp)
target_compile_features(app PRIVATE cxx_std_17)
add_dependencies(app shaders)

# executable dependencies
target_link_libraries(app logc)

# ikura lib ----------
add_subdirectory(./src/lib/ikura)

# ikura dependencies
target_link_libraries(ikura vulkan glfw)
target_link_libraries(ikura dl pthread X11 Xxf86vm Xrandr Xi)
target_link_libraries(renderEngine logc)

# link ikura to executable
target_link_libraries(app ikura)
