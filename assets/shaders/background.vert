attribute vec4 Position;
uniform mat4 model;
uniform int time;

varying vec2 pos;

void main()
{
	vec4 P = model * Position;
	
	pos = P.xy;
	gl_Position = P;
}