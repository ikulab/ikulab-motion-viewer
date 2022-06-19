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

# vulkan library
if(WIN32)
	find_library(vulkan_lib
		NAMES vulkan vulkan-1
		PATHS ${CMAKE_SOURCE_DIR}/VULKAN-SDK/lib
	)

	if(NOT vulkan_lib)
		message(FATAL_ERROR "cannot find vulkan library.")
	endif()

	link_libraries(vulkan_lib)
elseif(UNIX)
	find_package(Vulkan REQUIRED)
endif()

# glfw3 library
if(WIN32)
	find_library(glfw3_lib
		NAMES glfw3
		PATHS ${CMAKE_SOURCE_DIR}/glfw3-lib/lib-vc2022
	)

	if(NOT glfw3_lib)
		message(FATAL_ERROR "cannot find glfw3 library.")
	endif()

	link_libraries(glfw3_lib)

elseif(UNIX)
	find_package(glfw3 3.3 REQUIRED)
endif()

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

add_subdirectory(./src/shape)
target_link_libraries(shape)

add_subdirectory(./src/util)
target_link_libraries(bvh_parser)

add_library(animator STATIC ./src/animator.cpp)
target_link_libraries(animator shape)
target_link_libraries(animator bvh_parser)
target_link_libraries(app animator)

add_subdirectory(./includes/imgui)
target_link_libraries(app imgui)
