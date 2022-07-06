cmake_minimum_required(VERSION 3.16)
project(
	window
	VERSION 0.1.0	
	LANGUAGES CXX
)

file(GLOB NATIVE_WINDOW_SRC nativeWindow/*.cpp)
file(GLOB VIRTUAL_WINDOW_SRC virtualWindow/*.cpp)

add_library(nativeWindow STATIC ${NATIVE_WINDOW_SRC})
add_library(virtualWindow STATIC ${VIRTUAL_WINDOW_SRC})

add_library(window STATIC window.cpp windowInputContext.cpp)
target_link_libraries(window nativeWindow virtualWindow)
