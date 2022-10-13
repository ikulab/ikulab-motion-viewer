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

option(USE_CLANG "set CXX compiler to Clang" TRUE)

if(USE_CLANG)
	set(CMAKE_CXX_COMPILER clang++)
	set(CMAKE_C_COMPILER clang)
	add_compile_options(
		-Wno-nullability-completeness
	)
else()
	set(CMAKE_CXX_COMPILER gcc)
	set(CMAKE_C_COMPILER gcc)
endif()

if("${BUILD_TYPE}" STREQUAL "Release")
	add_compile_options(-DNODEBUG)
endif()

# external libraries ----------
find_package(glfw3 3.3 REQUIRED)
find_package(ikura)

add_subdirectory(${CMAKE_SOURCE_DIR}/includes/easylogging++)
add_subdirectory(${CMAKE_SOURCE_DIR}/includes/imgui)

# ikulab-motion-viewer app ----------
add_subdirectory(./src/app)

# app dependencies
target_link_libraries(app easyloggingpp imgui)
target_link_libraries(app ikura glfw)

# TODO: delete them
target_link_libraries(motionUtil ikura)