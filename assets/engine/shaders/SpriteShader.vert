#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

uniform mat4 view;
uniform mat4 model;

out vec2 tex_coord;
out vec3 normal;
out vec3 position;

void main()
{
	vec4 pos = model * vec4(a_position, 1.0);
	
	tex_coord = a_position.xy * vec2(0.5) + vec2(0.5);
	normal = a_normal;
	position = pos.xyz;	
	
	gl_Position = view * pos;
}