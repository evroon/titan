#version 330 core

attribute vec3 a_position;

uniform mat4 model;

out vec2 pos;

void main()
{
	pos = a_position.xy;
	
	gl_Position = model * vec4(pos, 0.0, 1.0);
}