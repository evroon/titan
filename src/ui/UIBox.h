#pragma once

#include "Control.h"
#include "world/Mesh.h"

class UIBox : public Control
{
public:
	UIBox();
	~UIBox();

	void set_color(const Color &p_color);
	Color get_color() const;

	void prepare_draw();
	void draw_mesh();

	void draw();

private:
	Color color;
	SimpleMesh *mesh_2d;
	Shader *shader;
	Transform transform;
};