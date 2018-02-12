#include "Raycaster.h"

#include "Camera.h"
#include "Terrain.h"
#include "graphics/Viewport.h"

#include "core/WindowManager.h"


Raycaster::Raycaster(Viewport* p_viewport)
{
	viewport = p_viewport;
	camera = viewport->get_world()->get_active_camera();
}

Raycaster::~Raycaster()
{
}

void Raycaster::set_camera(Camera* p_camera)
{
	camera = p_camera;
}

Camera* Raycaster::get_camera() const
{
	return camera;
}

void Raycaster::set_viewport(Viewport* p_viewport)
{
	viewport = p_viewport;
}

Viewport* Raycaster::get_viewport() const
{
	return viewport;
}

vec3 Raycaster::raycast(const vec2& p_screen_pos)
{
	rect2 window = viewport->get_area();

	if (!rect2(1.0).is_in_box(p_screen_pos / viewport->get_size()) )
		return vec3();

	vec3 dir = get_dir(p_screen_pos);
	float step_size = 1.0f;
	int max_it = 10000;

	World* world = viewport->get_world();
	Terrain* t = world->get_child("terrain")->cast_to_type<Terrain*>();

	if (!t)
		return vec3();

	vec3 pos = camera->get_pos();

	vec3 p = pos + dir * 20.0f;
	vec4 s = camera->get_projection_matrix() * camera->get_view_matrix() * vec4(p, 0.0f);
	vec2 m = MOUSE->get_position() - window.pos;
	//T_LOG(m.to_string() + " and " + s.get_xy().to_string());
	//T_LOG(dir.to_string());

	//terrain only
	for (int i = 0; i < max_it; i++)
	{
		float height = t->get_height(pos.get_xy());
		height = 0;

		if (pos.z <= height)
		{
			return vec3(pos.get_xy(), height);
		}

		pos += dir * step_size;
	}

	return vec3();
}

vec3 Raycaster::get_dir(const vec2& p_pos)
{
	vec2 size = viewport->get_size();

	vec2 ndc = (p_pos * vec2(1, 1)) / size;
	vec4 clipped = vec4(ndc, -1.0f, 1.0f);
	
	mat4 proj_inv = camera->get_projection_matrix().inverse();
	vec4 eye_coords = proj_inv * clipped;
	eye_coords = vec4(eye_coords.get_xy(), -1.0f, 0.0f);

	mat4 view_inv = camera->get_view_matrix().inverse();
	vec4 world_coords = view_inv * eye_coords;
	
	return world_coords.get_xyz().normalize();
}
