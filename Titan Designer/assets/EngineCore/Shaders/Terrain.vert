#version 330 core

layout (location = 0) in vec3 a_position;

uniform mat4 view;
uniform mat4 model;
uniform vec2 offset;

out vec2 texcoord;
out vec3 normal;
out vec3 pos;

void main()
{
	vec4 position = model * vec4(a_position + vec3(offset, 0.0), 1.0);
	vec4 plane = vec4(0, 0, 1, 0.0);	
	
	gl_ClipDistance[0] = dot(position, plane);
	
	texcoord = vec2(0, 0);
	normal = vec3(0, 1, 0);
	pos = position.xyz;
	
	gl_Position = view * position;
}