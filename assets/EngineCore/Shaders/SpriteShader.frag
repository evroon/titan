#version 330 core

uniform sampler2D tex;

uniform vec3 color_id;
uniform vec4 color;
uniform bool texture_enabled;
uniform vec4 texbounds;

in vec2 tex_coord;
in vec3 normal;
in vec3 position;

void main()
{
	float alpha = color.a;
	vec3 source = color.rgb;
		
	if (texture_enabled)
	{
		vec4 c = texture2D(tex, tex_coord * vec2(texbounds.y - texbounds.x, texbounds.w - texbounds.z) + texbounds.xz);
		alpha *= c.a;
		source = source * c.rgb;
	}
	
	if (alpha < 0.3)
		discard;
		
	gl_FragData[0] = vec4(source, alpha);
	gl_FragData[1] = vec4(position, 1.0);
	gl_FragData[2] = vec4(normal, 1.0);
	gl_FragData[3] = vec4(color_id, 1.0);
}