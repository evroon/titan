#pragma once

#include "worldobject.h"

#include "math/transform.h"
#include "math/quaternion.h"

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

class Viewport;

class Camera : public WorldObject
{
	OBJ_DEFINITION(Camera, WorldObject);

public:
	Camera();

	void update() override;

	void update_matrices();

	void activate();
	void deactivate();

	void look_at(const vec3& p_target, const vec3& p_up);
	void look_at(float p_pitch, float p_yaw, float p_roll);

	void set_projection(float p_fov, float p_near, float p_far);
	void set_ortho_projection(float p_near, float p_far, const vec2& p_size);

	void set_fov(float p_fov);
	float get_fov() const;

	void set_near(float p_near);
	float get_near() const;

	void set_far(float p_far);
	float get_far() const;

	void set_zoom(float p_zoom);
	float get_zoom() const;

	Viewport* get_viewport() const;

	void mult_zoom(float p_factor);

	bool get_orthogonal() const;

	const mat4& get_projection_matrix() const;
	const mat4& get_view_matrix() const;
	const mat4& get_final_matrix() const;

	mat4 get_inverse() const;

	float get_focal_length() const;

	vec3 get_forward() const;
	vec3 get_up() const;
	vec3 get_right() const;

	SIMPLE_GET_SET(vec2, ortho_size);

	static void bind_methods();

private:

	void update_projection_matrix();
	void update_view_matrix();
	void update_final_matrix();

	void handle_input();

	//projection data
	float fov;
	float near;
	float far;

	bool orthogonal;
	vec2 ortho_size;

	//view data
	vec3 target;
	vec3 up;

	Quaternion quaternion;

	vec2 pos;
	float zoom;

	mat4 projection_matrix;
	mat4 view_matrix;
	mat4 final_matrix;
};
