#version 330

attribute vec3 a_position;

out vec2 pos;

void main()
{
	pos = a_position.xy;
	
	gl_Position = vec4(a_position.xy, 0.0, 1.0);
}