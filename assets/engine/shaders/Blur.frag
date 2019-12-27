uniform sampler2D tex;
varying vec2 texcoords;

uniform vec2 step;

const float range = 4.0;

void main()
{
    vec4 sum = vec4(0.0);

    for (float c = -range; c <= range; c += 1.0) {
        sum += texture2D(tex, texcoords + step * c);
    }

    sum /= range * 2.0 + 1.0;

	gl_FragColor = vec4(sum.xyz, 1.0);
}