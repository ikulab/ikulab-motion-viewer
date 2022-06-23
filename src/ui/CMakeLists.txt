cmake_minimum_required(VERSION 3.16)
project(
	ui
	VERSION 0.1.0	
	LANGUAGES CXX
)

file(GLOB_RECURSE SOURCES *.cpp)

add_library(ui STATIC ${SOURCES})