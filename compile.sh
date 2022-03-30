#!/bin/bash

cd shaders

/usr/local/bin/glslc shader.vert -o bin/vert.spv
/usr/local/bin/glslc shader.frag -o bin/frag.spv
