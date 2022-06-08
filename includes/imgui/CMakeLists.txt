cmake_minimum_required(VERSION 3.16)
project(
	imgui
	VERSION 0.1.0
	LANGUAGES CXX
)

file(GLOB SOURCES *.cpp)
add_library(imgui STATIC ${SOURCES})

target_include_directories(imgui INTERFACE ../)