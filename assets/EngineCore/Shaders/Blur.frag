uniform sampler2D tex;
varying vec2 texcoords;

uniform vec2 step, direction;

void main()
{
    vec4 sum;
	
    sum += texture2D(tex, texcoords + direction * step * 4) * 0.0162162162;
    sum += texture2D(tex, texcoords + direction * step * 3) * 0.0540540541;
    sum += texture2D(tex, texcoords + direction * step * 2) * 0.1216216216;
    sum += texture2D(tex, texcoords + direction * step * 1) * 0.1945945946;

    sum += texture2D(tex, texcoords) * 0.2270270270;

    sum += texture2D(tex, texcoords + direction * -step * 1) * 0.1945945946;
    sum += texture2D(tex, texcoords + direction * -step * 2) * 0.1216216216;
    sum += texture2D(tex, texcoords + direction * -step * 3) * 0.0540540541;
    sum += texture2D(tex, texcoords + direction * -step * 4) * 0.0162162162;

	gl_FragColor = vec4(sum.xyz, 1.0);	
}