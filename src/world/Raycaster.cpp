#include "Raycaster.h"

#include "Camera.h"

#include "core/WindowManager.h"

Raycaster::Raycaster(Camera* p_camera)
{
	camera = p_camera;
}


Raycaster::~Raycaster()
{
}

vec3 Raycaster::raycast(const vec2& p_pos)
{
	vec3 dir = get_dir(p_pos);

	return vec3();
}

vec3 Raycaster::get_dir(const vec2 & p_pos)
{
	vec2 ndc = (vec2(2.0f) * p_pos) / vec2(to_float(WINDOWSIZE.x), to_float(WINDOWSIZE.y));
	vec4 clipped = vec4(ndc, -1.0f, 1.0f);

	mat4 proj_inv = camera->get_projection_matrix().inverse();
	vec4 eye_coords = proj_inv * clipped;
	eye_coords = vec4(eye_coords.get_xy(), -1.0f, 0.0f);

	mat4 view_inv = camera->get_view_matrix().inverse();
	vec4 world_coords = view_inv * eye_coords;
	world_coords.normalize();

	return world_coords.get_xyz();
}
