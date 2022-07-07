cmake_minimum_required(VERSION 3.16)
project(
	ikura
	VERSION 0.1.0	
	LANGUAGES CXX
)

add_library(ikura STATIC ikura.cpp)

add_subdirectory(./window)
target_link_libraries(ikura window)

add_subdirectory(./engine)
target_link_libraries(ikura appEngine renderEngine uiEngine)

target_include_directories(ikura INTERFACE ../)