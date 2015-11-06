#version 330 core

in vec2 f_color;

out vec4 color;

void main(void) {
	color = vec4(f_color, 1.0, 1.0);
}
