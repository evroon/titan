uniform sampler2D tex;
uniform int time;

varying vec2 pos;

void main()
{
	float freq = 1.0 / 100.0;
	float kvector = 10.0;
	
	float d1 = pos.x * pos.x;
	float d2 = pos.y * pos.y;
	
	float wave1 = sin(d1 * kvector - freq * time);
	float wave2 = sin(d2 * kvector - freq * time);
	
	float wave = (wave1 + wave2) / 2.0;
	
	vec3 color1 = vec3(0.0, 1.0, 0.0);
	vec3 color2 = vec3(0.0, 0.8, 0.0);
	
	if (wave > 0.0)
		gl_FragColor = vec4(color1, 1.0);
	else
		gl_FragColor = vec4(color2, 1.0);
	
	gl_FragColor = vec4(mix(color1, color2, (wave1 + 1.0) / 2.0), 1.0);
}