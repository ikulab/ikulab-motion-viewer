cmake_minimum_required(VERSION 3.16)
project(
	vk_mem_alloc
	VERSION 0.1.0
	LANGUAGES C
)

add_library(vk_mem_alloc INTERFACE)

target_include_directories(vk_mem_alloc INTERFACE ./)