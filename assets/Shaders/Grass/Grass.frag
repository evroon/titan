#version 330

uniform sampler2D tex;
uniform vec4 color;

in struct geom_data
{
    vec2 tex_coord;
    vec3 normal;
    vec3 position;
} geom_out;

out vec4 albedo;
out vec4 position;
out vec4 normal;
out vec4 material;

void main()
{		
	vec4 sampled = texture2D(tex, geom_out.tex_coord);
	vec3 source = sampled.rgb;
	
	if (sampled.w < 0.3)
		discard;
		
	gl_FragData[0] = vec4(source, 1.0);
	gl_FragData[1] = vec4(geom_out.position, 1.0);
	gl_FragData[2] = vec4(geom_out.normal, 1.0);
	gl_FragData[3] = vec4(vec3(1.0, 0.0, 0.0), 1.0);
}