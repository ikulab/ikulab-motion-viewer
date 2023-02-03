project(
	tinyfiledialogs
	VERSION 3.8.8
	LANGUAGES C
)

add_library(tinyfiledialogs STATIC ./tinyfiledialogs.c)

set_target_properties(tinyfiledialogs PROPERTIES COMPILE_OPTIONS "")