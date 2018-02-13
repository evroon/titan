#version 330

uniform sampler2D tex;
uniform vec4 color;
uniform vec4 texbounds;
uniform bool texture_enabled;

varying vec2 texcoords;

void main()
{
	if (texture_enabled)
		gl_FragColor = texture2D(tex, texcoords * vec2(texbounds.y - texbounds.x, texbounds.w - texbounds.z) + texbounds.xz) * color;
	else
		gl_FragColor = color;
}