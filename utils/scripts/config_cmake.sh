#!/bin/bash

cmake -H. -S . -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
