cmake_minimum_required(VERSION 3.16)
project(
	engine
	VERSION 0.1.0	
	LANGUAGES CXX
)

find_package(glfw3 3.3 REQUIRED)


add_library(appEngine STATIC appEngine.cpp)

add_library(renderEngine STATIC renderEngine.cpp)

add_library(uiEngine STATIC uiEngine.cpp)