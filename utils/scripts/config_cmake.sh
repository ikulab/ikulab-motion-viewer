#!/bin/bash

cmake -H.\
	-S .\
	-B build\
	-G Ninja\
	-D BUILD_TYPE=Debug\
	-DCMAKE_EXPORT_COMPILE_COMMANDS=YES
