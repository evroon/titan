#version 330

uniform sampler2D tex;
uniform vec4 color;
uniform vec4 texbounds;
uniform bool texture_enabled;

varying vec2 texcoords;

void main()
{
	if (texture_enabled)
		gl_FragData[0] = texture2D(tex, texcoords * vec2(texbounds.y - texbounds.x, texbounds.w - texbounds.z) + texbounds.xz) * color;
	else
		gl_FragData[0] = vec4(color);
		
	if (gl_FragData[0].a < 0.3)
		discard;
	
	gl_FragData[3] = vec4(0.2, 0.2, 0.2, 1.0);
}