#!/bin/bash

find ./src -name "*.?pp" | xargs clang-format -i -style=file