cmake_minimum_required(VERSION 3.16)
project(
	app
	VERSION 0.1.0
	LANGUAGES CXX
)

add_executable(app ./main.cpp ./app.cpp)
target_compile_features(app PRIVATE cxx_std_17)
