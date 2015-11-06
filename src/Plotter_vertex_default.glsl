#version 330 core

layout (location = 0) in vec3 position;
varying vec4 f_color;
uniform float offset_x;
uniform float scale_x;
uniform float offset_y;
uniform float scale_y;
uniform float offset_z;
uniform float scale_z;

void main(void) {
	gl_Position = vec4((position.x + offset_x) * scale_x, (position.y + offset_y) * scale_y, (position.z + offset_z) * scale_z, 1.0);
	f_color = vec4(position.xy / 2.0 + 0.5, 1, 1);
}
