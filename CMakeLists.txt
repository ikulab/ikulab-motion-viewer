cmake_minimum_required(VERSION 3.16)
project(
	ikulab-motion-viwer
	VERSION 0.1.0	
	LANGUAGES CXX
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

exec_program(
	make ./tool
)

add_compile_options(
	-std=c++17 -fdiagnostics-color=always -g
)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_executable(app ./src/main.cpp ./src/app.cpp ./src/base.cpp)
target_compile_features(app PRIVATE cxx_std_17)

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
