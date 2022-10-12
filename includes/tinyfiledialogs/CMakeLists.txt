project(
	tinyfiledialogs
	VERSION 3.8.8
	LANGUAGES C
)

add_library(tinyfiledialogs STATIC ./tinyfiledialogs.c)
target_include_directories(tinyfiledialogs INTERFACE ./)

set_target_properties(tinyfiledialogs PROPERTIES COMPILE_OPTIONS "")