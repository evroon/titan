attribute vec4 Position;
uniform mat4 model;
uniform vec2 lightpos;
uniform float radius;

varying vec2 pos;

void main()
{
	vec4 newpos;
	if (Position.y > 0)
	{
		newpos = model * vec4(Position.x, -1, Position.zw);
		newpos.xy -= lightpos;
		newpos.xy = normalize(newpos.xy);
		newpos.xy *= 100;
	}
	else
		newpos = model * Position;
	
	pos = newpos.xy - lightpos;
	gl_Position = newpos;
}