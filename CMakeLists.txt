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

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# shader compile
add_custom_target(
	shaders
	make
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tool
	DEPENDS ./shaders/shader.frag ./shaders/shader.vert
	COMMENT "compiling shaders"
)

add_executable(app ./src/main.cpp ./src/app.cpp ./src/base.cpp)
target_compile_features(app PRIVATE cxx_std_17)
add_dependencies(
	app
	shaders
)

target_link_libraries(app glfw)
target_link_libraries(
	app vulkan dl pthread X11 Xxf86vm Xrandr Xi
)

add_subdirectory(./includes/imgui)
target_link_libraries(app imgui)

add_subdirectory(./src/shape)
target_link_libraries(shape)

add_subdirectory(./src/util)
target_link_libraries(bvh_parser)

add_library(animator STATIC ./src/animator.cpp)
target_link_libraries(animator shape)
target_link_libraries(animator bvh_parser)
target_link_libraries(app animator)

add_subdirectory(./src/ui)
target_link_libraries(ui)
target_link_libraries(ui imgui)

add_subdirectory(./src/context)
target_link_libraries(context)
target_link_libraries(context imgui)