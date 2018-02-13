uniform sampler2D tex;
uniform vec4 color;
varying vec2 pos;

void main()
{
	vec3 a = vec3(.01, .15, .9);
	float dist = 1.0 - length(pos);

	float intensity = a.x + a.y * dist + a.z * dist * dist;
	intensity = clamp(intensity, 0, 1);
	
	gl_FragColor = vec4(intensity);
}