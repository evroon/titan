#version 330

uniform sampler2D tex;
uniform float threshold;

in vec2 texcoords;

out vec4 final_color;

void main()
{
	vec3 color = texture(tex, texcoords).rgb;
	
	if (color.r < threshold || color.g < threshold || color.b < threshold)
		discard;
	
	final_color = vec4(color, 1.0);	
}