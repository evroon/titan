#version 330 core

in vec3 position;

void main()
{
	gl_FragData[0] = vec4(position, 1.0);
}