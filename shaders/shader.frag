#version 450

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec3 flagColor;

void main() {
	outColor = vec4(flagColor, 1.0);
}