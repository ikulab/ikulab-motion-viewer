cmake_minimum_required(VERSION 3.16)
project(
	engine
	VERSION 0.1.0	
	LANGUAGES CXX
)

find_package(glfw3 3.3 REQUIRED)

add_library(appEngine STATIC appEngine.cpp)

file(GLOB RENDER_ENGINE_SOURCE renderEngine/*.cpp)
add_library(renderEngine STATIC ${RENDER_ENGINE_SOURCE})

add_library(uiEngine STATIC uiEngine.cpp)