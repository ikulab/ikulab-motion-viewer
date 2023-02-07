#!/bin/bash

export CC=/usr/bin/clang
export CXX=/usr/bin/clang++

cmake -H.\
	-S .\
	-B build\
	-G Ninja\
	-D BUILD_TYPE=Debug\
	-DCMAKE_EXPORT_COMPILE_COMMANDS=YES
