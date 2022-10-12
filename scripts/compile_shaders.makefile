all: ../shaders/bin/frag.spv ../shaders/bin/vert.spv

../shaders/bin/vert.spv: ../shaders/shader.vert
	glslc ../shaders/shader.vert -o ../shaders/bin/vert.spv

../shaders/bin/frag.spv: ../shaders/shader.frag
	glslc ../shaders/shader.frag -o ../shaders/bin/frag.spv