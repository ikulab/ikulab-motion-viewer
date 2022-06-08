cmake_minimum_required(VERSION 3.16)
project(
	shape
	VERSION 0.1.0	
	LANGUAGES CXX
)

file(GLOB_RECURSE SOURCES *.cpp)

add_library(shape STATIC ${SOURCES})