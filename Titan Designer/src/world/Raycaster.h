#pragma once

#include "math/Math.h"

#include "world/World.h"

class Camera;

class Raycaster
{
public:
	Raycaster(Viewport* p_viewport);
	~Raycaster();

	void set_viewport(Viewport* p_viewport);
	Viewport* get_viewport() const;

	void set_camera(Camera* p_camera);
	Camera* get_camera() const;

	vec3 raycast(const vec2& p_screen_pos);

private:
	vec3 get_dir(const vec2& p_pos);
	
	Camera* camera;
	Viewport* viewport;
};

