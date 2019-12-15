#version 330 core

uniform sampler3D noise;
uniform sampler2D g_albedo;
uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_material;
uniform sampler2D shadow_map;
uniform sampler2D godray_tex;

uniform mat4 inv_proj;
uniform mat4 inv_view;
uniform mat4 light_matrix;

uniform vec3 cam_pos;
uniform vec3 light_pos;
uniform bool clouds_enabled;
uniform bool lighting_enabled;
uniform vec3 light_dir;
uniform vec3 ambient;
uniform vec3 view_pos;
uniform vec3 light_color;
uniform float specular_strength;
uniform float specular_power;
uniform bool fog_enabled;
uniform float fog_density;
uniform float fog_gradient;
uniform vec3 sky_color;
uniform bool godray_enabled;

in vec2 pos;
out vec4 final_color;
	
vec3 scale = vec3(10, 10, 10);
vec3 clouds_pos = vec3(45, 45, 120);
int step_count = 8;
int light_samples = 5;
float cutoff = 0.55;


//http://prideout.net/blog/?p=64
struct Ray
{
    vec3 Origin;
    vec3 Dir;
};

struct AABB
{
    vec3 Min;
    vec3 Max;
};

vec3 get_dir()
{
	vec2 window_size = vec2(2400, 1300);
	vec2 ndc = 2.0 * gl_FragCoord.xy / window_size - 1.0;
	vec4 clipped = vec4(ndc, -1.0, 1.0);
	vec4 eye_coords = inv_proj * clipped;
	eye_coords = vec4(eye_coords.xy, -1.0, 0.0);
	vec4 world_coords = inv_view * eye_coords;
	
	return normalize(world_coords.xyz);	
}

bool IntersectBox(Ray r, AABB aabb, out float t0, out float t1)
{
    vec3 invR = 1.0 / r.Dir;
    vec3 tbot = invR * (aabb.Min-r.Origin);
    vec3 ttop = invR * (aabb.Max-r.Origin);
    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);
    vec2 t = max(tmin.xx, tmin.yz);
    t0 = max(t.x, t.y);
    t = min(tmax.xx, tmax.yz);
    t1 = min(t.x, t.y);
    return t0 <= t1;
}

float bandpass(float x, float start, float edge1, float edge2, float end)
{
	if (x < start)
		return 0.0;
	if (x > end)
		return 0.0;
	if (x < edge1)
		return smoothstep(start, edge1, x);
	if (x > edge2)
		return smoothstep(-end, -edge2, -x);
	
	return 1.0;
}

float get_density(vec3 pos)
{
	if (pos.x < clouds_pos.x || pos.x > clouds_pos.x + scale.x)
		return 0.0;
	
	if (pos.y < clouds_pos.y || pos.y > clouds_pos.y + scale.y)
		return 0.0;
	
	if (pos.z < clouds_pos.z || pos.z > clouds_pos.z + scale.z)
		return 0.0;
	
	float height = (pos.z - clouds_pos.z) / scale.z;
	//height /= 1.0 - height;
	
	float stratus = bandpass(height, 0.0, 0.1, 0.7, 0.8);
	
	if (stratus == 0.0)
		return 0.0;
	
	vec4 sample = texture(noise, (pos - clouds_pos) / scale * vec3(1.0));
	vec4 sample2 = texture(noise, (pos - clouds_pos) / scale * vec3(4.0));
	
	float perlin = sample.r;
	float low = sample2.g;
	float medium = sample2.b;
	float high = sample2.a;
	
	float noise_contrib = 4.0;
	
	float noise_term =  -(1.0 - low) / 4 - (1.0 - medium) / 8 - (1.0 - high) / 16.0;		
	float noise_value = perlin + noise_term * height * noise_contrib;
		
	noise_value *= stratus;
	noise_value -= cutoff;
	
	return noise_value * 5.0;
}

float ray_march()
{
	if (!clouds_enabled)
		return 0.0f;
	
	vec3 dir = get_dir();
    Ray eye = Ray(cam_pos, normalize(dir));
    AABB aabb = AABB(clouds_pos, clouds_pos + scale);
    float near, far;
    IntersectBox(eye, aabb, near, far);
	
    if (near < 0.0)
		near = 0.0;
	
	if (far < 0.0)
		return 0.0;
	
	float step_size = 1.0 / step_count;
    vec3 start = cam_pos + dir * near;
    vec3 stop = cam_pos + dir * far;
	
	vec3 pos = start;
    vec3 step = (stop - start) * step_size;
	
	float value = 0.0;
	
	for (int i = 0; i < step_count; i++)
	{
		float density = get_density(pos);
		
		if (density > 0.0)
		{
			vec3 l_pos = pos;
			vec3 l_dir = vec3(0.0, 0.0, 1.0);
			l_dir = normalize(l_dir);
			
			float light_value = 0.0;
		
			for (int c = 0; c < light_samples; c++)
			{
				l_pos += l_dir * 0.7;
				
				float l_density = get_density(l_pos);
				
				if (l_density > 0.0)
					light_value += l_density * 1.0;
				
				//if (light_value > 3.0)
				//	break;
			}
			
			value = value + (density - light_value) / step_count ;
		
			if (value >= 1.0)
				return 1.0;
		}
		
		pos += step;
    }
	
	return value * 20.0;
}

vec2 tex_coords;
vec3 albedo;
vec3 position;
vec3 normal;
vec3 material;

vec3 get_specular()
{
	vec3 view_dir = normalize(view_pos - position);
	vec3 reflect_dir = reflect(-vec3(0, 0, -1), normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), specular_power);
	return specular_strength * spec * light_color;
}

vec3 lighting()
{
	if (!lighting_enabled)
		return vec3(1.0);
	
	float diff = dot(normal, normalize(light_dir));
	
	vec3 diffuse = diff * vec3(0.5);
	vec3 specular = get_specular() * vec3(0.001);
	
	return ambient + diffuse + specular;
}


vec3 get_godray()
{
	if (!godray_enabled)
		return vec3(0.0);
	
	return texture2D(godray_tex, tex_coords).rgb;
}

float get_fog()
{
	if (!fog_enabled)
		return 1.0;
	
	float distance = length(view_pos - position);
	float visibility = exp(-pow((distance * fog_density), fog_gradient));
	visibility = clamp(visibility, 0.0, 1.0);
	return visibility;
}

float shadow_calc(vec4 pos_light_space)
{
    vec3 projCoords = pos_light_space.xyz / pos_light_space.w;
    projCoords = projCoords * 0.5 + 0.5;
	
	//if (projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
	//	return 0.0;
	
    float closestDepth = texture2D(shadow_map, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.0;
	float shadow = currentDepth - bias > closestDepth ? 0.6 : 0.0;

    return shadow;
}

void main()
{
	tex_coords = pos * vec2(0.5) + vec2(0.5);

	albedo = texture2D(g_albedo, tex_coords).rgb;
	material = texture2D(g_material, tex_coords).rgb;

	if (material.xy == vec2(0.0) && material.z > 0.5 && material.z < 1.0) {
		final_color = vec4(albedo, 1.0);
		return;
	}

	position = texture2D(g_position, tex_coords).rgb;
	normal = texture2D(g_normal, tex_coords).rgb;
	
	if (material == vec3(1.0, 0.0, 0.0) || material == vec3(1.0, 0.0, 1.0) || material == vec3(0.2, 0.2, 0.2))
	{
		final_color = vec4(albedo, 1.0);
		return;
	}
	
    vec4 pos_light_space = light_matrix * vec4(position.xyz, 1.0);
    float shadow = 1.0 - shadow_calc(pos_light_space);
	
	vec3 finalcolor = albedo * lighting() * shadow;
	finalcolor = mix(sky_color, finalcolor, get_fog());
		
	//if (clouds_enabled)
	//	finalcolor = finalcolor + ray_march();	
	
	final_color = vec4(finalcolor + get_godray(), 1.0);
}
