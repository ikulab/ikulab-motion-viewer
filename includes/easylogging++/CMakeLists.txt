cmake_minimum_required(VERSION 3.16)
project(
	easyloggingpp
	VERSION 0.1.0
	LANGUAGES CXX
)

add_library(easyloggingpp STATIC ./easylogging++.cc)

target_include_directories(easyloggingpp INTERFACE ./)