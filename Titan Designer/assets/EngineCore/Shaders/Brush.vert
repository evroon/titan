#version 330

attribute vec3 a_position;

out vec2 pos;

void main()
{
	vec2 p = a_position.xy;
	pos = p;
	p /= 3.0;
	
	gl_Position = vec4(p, 0.0, 1.0);
}