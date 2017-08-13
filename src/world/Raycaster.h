#pragma once

#include "math\Math.h"

class Camera;

class Raycaster
{
public:
	Raycaster(Camera* p_camera);
	~Raycaster();


	vec3 raycast(const vec2& p_pos);

private:
	vec3 get_dir(const vec2& p_pos);


	Camera* camera;
};

