#include "Camera.h"

#include "graphics/Viewport.h"
#include "World.h"
#include "graphics/Renderer.h"

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

Camera::Camera()
{
	near = 0.5f;
	far = 1000.0f;

	target = vec3(0, 0, -1.0f);
	up = vec3(0, 1, 0);

	fov = 45;

	zoom = 1.0f;
	pos = vec2();

	orthogonal = false;
}

void Camera::update()
{
	update_projection_matrix();
	update_view_matrix();
	update_final_matrix();
}

void Camera::activate()
{
	RENDERER->activate_projection_matrix(projection_matrix);
	RENDERER->activate_view_matrix(view_matrix);
}

void Camera::deactivate()
{
	RENDERER->deactivate_projection_matrix();
	RENDERER->deactivate_view_matrix();
}

void Camera::look_at(const vec3& p_target, const vec3& p_up)
{
	target = p_target;
	up = p_up;

	update_view_matrix();
	update_final_matrix();
}

void Camera::look_at(float p_pitch, float p_yaw, float p_roll)
{
	set_rotation(vec3(p_pitch, p_yaw, p_roll));
}

void Camera::set_projection(float p_fov, float p_near, float p_far)
{
	fov = p_fov;
	near = p_near;
	far = p_far;

	orthogonal = false;

	update_projection_matrix();
	update_final_matrix();
}

void Camera::set_ortho_projection(float p_near, float p_far)
{
	near = p_near;
	far = p_far;

	orthogonal = true;

	update_projection_matrix();
	update_final_matrix();
}

Viewport* Camera::get_viewport() const
{
	if (!get_world() || !get_world()->get_viewport())
	{
		T_ERROR("Could not get viewport");
		return NULL;
	}
	return get_world()->get_viewport();
}

void Camera::update_projection_matrix()
{
	rect2 area = get_viewport()->get_size();
	rect2 window = { pos, area.get_size() * zoom };

	if (!orthogonal)
		projection_matrix.perspective(window, fov, window.size.y / window.size.x, near, far);
	else
		projection_matrix.orthographic(window, near, far);
}

void Camera::update_view_matrix()
{
	quat key_quat = quat(get_rotation());
	quat camera_quat;

	camera_quat = key_quat * camera_quat;
	camera_quat.normalize();
	mat4 rotate = camera_quat;

	mat4 translate;
	translate.translate(vec3(-1.0f) * get_pos());

	mat4 look_at;
	look_at.look_at(get_pos(), get_pos() + vec3(0, 1, 0), vec3(0, 0, 1));

	view_matrix = rotate * look_at;
}

void Camera::update_final_matrix()
{
	final_matrix = projection_matrix * view_matrix;
}

void Camera::set_fov(float p_fov)
{
	fov = p_fov;
}

float Camera::get_fov() const
{
	return fov;
}

void Camera::set_near(float p_near)
{
	near = p_near;
}

float Camera::get_near() const
{
	return near;
}

void Camera::set_far(float p_far)
{
	far = p_far;
}

float Camera::get_far() const
{
	return far;
}

void Camera::set_zoom(float p_zoom)
{
	zoom = p_zoom;
}

float Camera::get_zoom() const
{
	return zoom;
}

void Camera::mult_zoom(float p_factor)
{
	zoom *= p_factor;
}

bool Camera::get_orthogonal() const
{
	return orthogonal;
}

const mat4& Camera::get_projection_matrix() const
{
	return projection_matrix;
}

const mat4& Camera::get_view_matrix() const
{
	return view_matrix;
}

const mat4& Camera::get_final_matrix() const
{
	return final_matrix;
}

mat4 Camera::get_inverse() const
{
	mat4 view_inverse = view_matrix.inverse();
	mat4 projection_inverse = projection_matrix.inverse();

	return view_inverse * projection_inverse;
}
