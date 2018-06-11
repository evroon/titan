#version 330

uniform vec2 size;
uniform float strength;

out vec4 final_color;

void main()
{	
	float height = 1.0;
	
	final_color = vec4(height, height, height, 0.1);
}