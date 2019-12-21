#version 330 core

layout (location = 0) in vec3 a_position;

uniform mat4 view;
uniform mat4 model;

out vec3 position;
out vec4 screenspace_position;

void main()
{
	vec4 pos = model * vec4(a_position, 1.0);
	
	position = pos.xyz;
	
	vec4 screen_space = view * pos;
	screenspace_position = screen_space;
	
	gl_Position = screen_space;
}