attribute vec4 a_position;

varying vec2 texcoords;

uniform sampler2D tex;
uniform mat4 model;
uniform bool texture_enabled;
uniform vec4 color, texbounds;

void main()
{
	vec4 pos = vec4(a_position.xy, 0.0, 1.0);
	texcoords = pos.xy * vec2(0.5) + vec2(0.5);
	gl_Position = model * pos;
}