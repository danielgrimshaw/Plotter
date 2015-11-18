#version 330 core

layout (location = 0) in vec3 position;

out vec2 f_color;

uniform mat4 model_transform;
uniform mat4 view;
uniform mat4 projection;

void main(void) {
	gl_Position = projection * view * model_transform * vec4(position, 1.0f);
	f_color = vec2(position.xy / 2.0 + 0.5);
}
