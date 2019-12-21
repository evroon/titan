#version 330 core

layout (location = 0) in vec3 a_position;

uniform mat4 view;
uniform vec3 camera_position;

out vec3 position;

void main()
{
	vec4 pos = vec4(a_position * 500.0, 1.0);
	
	position = pos.xyz;
	
	pos = pos + vec4(camera_position, 0.0);
	
	gl_Position = view * pos;
}