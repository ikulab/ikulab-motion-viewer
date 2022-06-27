cmake_minimum_required(VERSION 3.16)
project(
	context
	VERSION 0.1.0	
	LANGUAGES CXX
)

file(GLOB SOURCES *.cpp)

add_library(context STATIC ${SOURCES})