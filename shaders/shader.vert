#version 450

const int MAX_ID = 3;

layout(binding = 0) uniform ModelMat {
	mat4 model[MAX_ID];
} modelMat;

layout(binding = 1) uniform SceneMat {
	mat4 view;
	mat4 proj;
} sceneMat;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in uint inId;

layout(location = 0) out vec3 flagColor;

void main() {
	// gl_Position = ubos[0].proj * ubos[0].view * ubos[0].model * vec4(inPosition, 1.0);
	gl_Position = sceneMat.proj * sceneMat.view * modelMat.model[inId] * vec4(inPosition, 1.0);

	flagColor = inColor;
}