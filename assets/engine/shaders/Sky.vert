#version 330 core

layout (location = 0) in vec3 a_position;

uniform mat4 view;
uniform vec3 camera_position;

out vec3 position;

void main()
{
	position = a_position * 5000.0;
	gl_Position = view * vec4(position + camera_position, 1.0);
}