#version 330

layout (location = 0) in vec3 position;
varying vec4 f_color;
uniform float offset_x;
uniform float scale_x;

void main(void) {
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
	f_color = vec4(position.xy / 2.0 + 0.5, 1, 1);
}
