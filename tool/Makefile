all: ../shaders/bin/frag.spv ../shaders/bin/vert.spv ../shaders/bin/triangle.vert.spv ../shaders/bin/triangle.frag.spv 

../shaders/bin/triangle.vert.spv: ../shaders/triangle.vert
	glslc ../shaders/triangle.vert -o ../shaders/bin/triangle.vert.spv

../shaders/bin/triangle.frag.spv: ../shaders/triangle.frag
	glslc ../shaders/triangle.frag -o ../shaders/bin/triangle.frag.spv

../shaders/bin/vert.spv: ../shaders/shader.vert
	glslc ../shaders/shader.vert -o ../shaders/bin/vert.spv

../shaders/bin/frag.spv: ../shaders/shader.frag
	glslc ../shaders/shader.frag -o ../shaders/bin/frag.spv