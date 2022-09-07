#pragma once

#include "control.h"

#include "resources/texture.h"
#include "world/mesh.h"

class Image : public Control
{
	OBJ_DEFINITION(Image, Control);

public:
	Image();
	Image(Texture2D *p_texture);
	Image(const String &tex_name);
	~Image() { }

	enum CenteringType
	{
		LEFT,
		CENTER,
		RIGHT,

		DEFAULT
	};

	CenteringType centertype = CENTER;

	void set_bounds(vec4 p_bounds);
	void set_bounds_in_pixels(vec4 p_bounds);

	void set_color(const Color &p_color);
	Color get_color() const;

	void set_texture(Texture2D *p_texture);
	Texture2D* get_texture() const;

	void flip_vertically();
	void flip_horizontally();

	vec2 get_required_size() const override;

	vec2 get_texture_size() const;
	float get_texture_ratio() const;
	vec2 get_texture_upscale() const;

	void notification(int p_notification) override;

	Texture2D *texture;

private:
	Color color = Color::White;
	SimpleMesh* mesh_2d;
	Shader *shader;
	vec4 bounds;
};