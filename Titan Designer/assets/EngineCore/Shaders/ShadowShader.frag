uniform float radius;
varying vec2 pos;

void main()
{
	float intensity = 2.0 * radius - length(pos);
	intensity = clamp(intensity, 0.0, 1.0);
	gl_FragColor = vec4(vec3(0.4), 1.0);
}