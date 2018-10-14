#version 460 core
layout (location = 0) in vec3 pos; // position
layout (location = 1) in vec3 norm; // normal
layout (location = 2) in vec3 color; // color
layout (location = 3) in vec2 texCoord; // texture position

out vec4 outColor; // specify a color output to the fragment shader;

uniform mat4 PVM;
uniform mat4 model;
uniform float radius;

void main() {
	gl_Position = PVM * vec4(pos * radius, 1.f);
    outColor = vec4(color, 1.f);
}