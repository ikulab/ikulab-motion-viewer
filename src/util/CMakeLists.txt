cmake_minimum_required(VERSION 3.18)
project(
	ikulab-motion-viwer
	VERSION 0.1.0	
	LANGUAGES CXX
)

add_library(bvh_parser STATIC ./bvhParser.cpp)