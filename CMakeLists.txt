cmake_minimum_required(VERSION 3.16)
project(
	ikulab-motion-viwer
	VERSION 0.1.0
	LANGUAGES CXX
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(UNIX)
	add_compile_options(
		-std=c++17 -fdiagnostics-color=always -g
	)
endif()

# vulkan library
if(WIN32)
	set(ENV{VULKAN_SDK} ${CMAKE_SOURCE_DIR}/VULKAN-SDK)
endif()

message("test")
find_package(Vulkan REQUIRED)

# glfw3 library
if(WIN32)
	find_library(Glfw_LIBRARY
		NAMES glfw3
		PATHS ${CMAKE_SOURCE_DIR}/glfw3-lib/lib-vc2022
		REQUIRED
	)
	find_path(GLFW_INCLUDE_DIR
		NAMES GLFW/glfw3.h
		PATHS ${CMAKE_SOURCE_DIR}/glfw3-lib/include
	)

elseif(UNIX)
	find_package(glfw3 3.3 REQUIRED)
endif()

# glm path
if(WIN32)
	find_path(GLM_INCLUDE_DIR
		NAMES glm/glm.hpp
		PATHS ${CMAKE_SOURCE_DIR}/glm
	)
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

if(WIN32)
	include_directories(${Vulkan_INCLUDE_DIR})
	include_directories(${Glfw_INCLUDE_DIR})
	include_directories(${GLM_INCLUDE_DIR})
	target_link_libraries(app
		Vulkan_LIBRARY Glfw_LIBRARY
	)
endif()

if(UNIX)
	target_link_libraries(app
		dl pthread X11 Xxf86vm Xrandr Xi
	)
	target_link_libraries(app
		vulkan glfw
	)
endif()

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
