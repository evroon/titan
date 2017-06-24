#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <GL\glew.h>
#include <GL\freeglut.h>

#include "resources/Texture.h"
#include "resources/Shader.h"
#include "world/Sprite.h"
#include "world/Mesh.h"
#include "game/Scene.h"
#include "world/Camera.h"
#include "world/Primitives.h"
#include "FBO.h"
#include "FBOManager.h"
#include "VisualEffect.h"
#include "core/Stack.h"

#define RENDERER Renderer::get_singleton()

class Renderer : public Object
{
	OBJ_DEFINITION(Renderer, Object);

public:
	void Init();
	void CheckGLError();
	void Prepare();
	void Finish();
	void Resize(int width, int height);
	void Free();

	void switch_to_camera(Camera* p_camera);

	void activate_projection_matrix(const mat4& p_projection_matrix);
	void activate_view_matrix(const mat4 &p_view_matrix);

	void deactivate_camera();

	void deactivate_projection_matrix();
	void deactivate_view_matrix();

	const mat4& get_projection_matrix() const;
	const mat4& get_view_matrix() const;
	const mat4& get_final_matrix() const;

	void use_scissor(const rect2 &area);
	void stop_scissor();

	void use_depth_test(float p_near, float p_far);
	void stop_depth_test();

	void use_culling();
	void stop_culling();

	void use_blending();
	void stop_blending();

	static Renderer* get_singleton();

	static void bind_methods();

private:
	void update();

	static Renderer singleton;

	Camera *activecamera;
	Shader *activeshader;
	VisualEffect *viseffect;

	mat4 projection_matrix;
	mat4 view_matrix;
	mat4 final_matrix;

	FBO2D* render_buffer;

	Stack<mat4> projections;
	Stack<mat4> views;
};