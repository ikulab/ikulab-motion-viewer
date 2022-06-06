cmake_minimum_required(VERSION 3.18)
project(
	imgui
	VERSION 0.1.0
	LANGUAGES CXX
)

file(GLOB SOURCES *.cpp)

add_library(imgui STATIC ${SOURCES})