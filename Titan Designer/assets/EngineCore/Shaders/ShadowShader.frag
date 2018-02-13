uniform float radius;
varying vec2 pos;

void main()
{
	float intens = 2 * radius - length(pos);
	intens = clamp(intens, 0, 1);
	gl_FragColor = vec4(vec3(.4), 1.0);
}