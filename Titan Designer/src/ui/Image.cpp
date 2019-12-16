#include "Image.h"

#include "graphics/Renderer.h"
#include "core/ContentManager.h"
#include "world/Mesh.h"

Image::Image()
{
	mesh_2d = MeshHandler::get_singleton()->get_plane();
	shader = CONTENT->LoadShader("EngineCore/Shaders/Shader2D");
	bounds = vec4(0.0, 1.0, 0.0, 1.0);
}
Image::Image(Texture2D *p_texture)
{
	texture = p_texture;
	mesh_2d = MeshHandler::get_singleton()->get_plane();
	shader = CONTENT->LoadShader("EngineCore/Shaders/Shader2D");
	bounds = vec4(0.0, 1.0, 0.0, 1.0);
}
Image::Image(const String &tex_name) : Image(CONTENT->LoadTexture(tex_name))
{

}

void Image::set_bounds(vec4 p_bounds)
{
	bounds = p_bounds;
}

void Image::set_bounds_in_pixels(vec4 p_bounds)
{
	bounds = vec4(vec2(p_bounds.x, p_bounds.y) / get_texture_size().x, vec2(p_bounds.z, p_bounds.w) / get_texture_size().y);
}

void Image::set_color(const Color &p_color)
{
	color = p_color;
}
Color Image::get_color() const
{
	return color;
}

void Image::set_texture(Texture2D *p_texture)
{
	texture = p_texture;
}
Texture2D* Image::get_texture() const
{
	return texture;
}

void Image::flip_vertically()
{
	float temp = bounds.z;
	bounds.z = bounds.w;
	bounds.w = temp;
}

void Image::flip_horizontally()
{
	float temp = bounds.x;
	bounds.x = bounds.y;
	bounds.y = temp;
}

vec2 Image::get_required_size() const
{
	return get_texture_size();
}

vec2 Image::get_texture_size() const
{
	return texture->get_size();
}

float Image::get_texture_ratio() const
{
	vec2 size = get_texture_size();
	return size.y / size.x;
}

vec2 Image::get_texture_upscale() const
{
	vec2 size = get_texture_size();
	return{ size.x / get_area().size.x, size.y / get_area().size.y };
}

void Image::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		area = rect2(vec2(), vec2(100));

		draw_texture(texture, area, color, bounds);

		break;

	case NOTIFICATION_RESIZED:

		break;

	}
}