#!/bin/bash

cd `dirname $0`/..

export LIBRARY_PATH="/usr/local/lib"
cmake --build build -j 6
