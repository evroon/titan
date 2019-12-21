#version 330

attribute vec3 a_position;

uniform mat4 model;

out vec2 pos;

void main()
{
	vec2 p = a_position.xy;
	pos = p * 0.5 + 0.5;
	
	gl_Position = model * vec4(p, 0.0, 1.0);
}