#version 330 core

uniform sampler2D tex;

in vec2 texcoords;

uniform vec2 step;
uniform float range;

void main()
{
    vec4 sum = vec4(0.0);

    for (float c = -range; c <= range; c += 1.0) {
        sum += texture2D(tex, texcoords + step * c);
    }

    sum /= range * 2.0 + 1.0;

	gl_FragColor = vec4(sum.xyz, 1.0);
}