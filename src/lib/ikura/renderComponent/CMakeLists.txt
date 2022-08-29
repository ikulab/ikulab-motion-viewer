cmake_minimum_required(VERSION 3.16)
project(
	renderComponent
	VERSION 0.1.0	
	LANGUAGES CXX
)

add_library(renderComponent STATIC renderContent.cpp renderTarget.cpp)