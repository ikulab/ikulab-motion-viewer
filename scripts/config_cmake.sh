#!/bin/bash

cd `dirname $0`/..

cmake\
	-S .\
	-B build\
	-G Ninja\
	-C ./init-cc.cmake\
	-DCMAKE_EXPORT_COMPILE_COMMANDS=YES
