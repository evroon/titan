#version 330

uniform vec2 size;
uniform float strength;
uniform sampler2D tex;

in vec2 pos;

out vec4 final_color;

void main()
{	
	float height = 1.0;
	height = texture2D(tex, pos).r;
	
	final_color = vec4(height, height, height, strength);
}