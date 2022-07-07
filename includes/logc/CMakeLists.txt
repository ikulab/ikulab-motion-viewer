cmake_minimum_required(VERSION 3.16)
project(
	logc
	VERSION 0.1.0
	LANGUAGES C
)

add_library(logc STATIC ./log.c)

target_include_directories(logc INTERFACE ../)