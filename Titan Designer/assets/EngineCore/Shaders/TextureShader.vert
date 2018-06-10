#version 330 core

attribute vec4 a_position;

out vec2 texcoords;

uniform mat4 model;

void main()
{
	vec4 pos = vec4(a_position.xy, 0.0, 1.0);
	texcoords = pos.xy * vec2(0.5) + vec2(0.5);
	gl_Position = model * pos;
}