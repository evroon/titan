#version 330 core

layout (location = 0) in vec3 a_position;

uniform mat4 view;
uniform mat4 model;
uniform vec2 offset;

void main()
{
	vec4 position = model * vec4(a_position + vec3(offset, 0.0), 1.0);
	
	gl_Position = position;
}