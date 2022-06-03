all: ../shaders/bin/frag.spv ../shaders/bin/vert.spv

../shaders/bin/vert.spv: ../shaders/shader.vert
	/usr/local/bin/glslc ../shaders/shader.vert -o ../shaders/bin/vert.spv

../shaders/bin/frag.spv: ../shaders/shader.frag
	/usr/local/bin/glslc ../shaders/shader.frag -o ../shaders/bin/frag.spv