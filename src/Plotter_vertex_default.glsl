#version 330 core

layout (location = 0) in vec3 position;

out vec2 f_color;

uniform mat4 transform;

void main(void) {
	gl_Position = transform * vec4(position, 1.0);
	f_color = vec2(position.xy / 2.0 + 0.5);
}
