#include "ImageMap.h"

ImageMap::ImageMap()
{
	for (int c = 0; c < 9; c++)
	{
		Image* image = new Image;

		images.push_back(image);
		add_child(image);
	}
}

ImageMap::ImageMap(Texture2D *p_texture, SplitProperties::TexCoords p_tex_coords, SplitProperties::Positions p_positions) : ImageMap()
{
	texture = p_texture;
	properties.tex_coords = p_tex_coords;
	properties.positions = p_positions;
}

void ImageMap::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		for (Image* image : images)
			image->draw();

		break;

	case NOTIFICATION_RESIZED:

		build();

		break;
	}
}

void ImageMap::build(Texture2D *p_texture, SplitProperties::TexCoords p_tex_coords, SplitProperties::Positions p_positions)
{
	texture = p_texture;
	properties.tex_coords = p_tex_coords;
	properties.positions = p_positions;

	build();
}

void ImageMap::build()
{
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			Image* image = images[x * 3 + y];

			vec2 horiz_bounds = vec2(properties.tex_coords.h_off[x], properties.tex_coords.h_off[x + 1]);
			vec2 vert_bounds = vec2(properties.tex_coords.v_off[y], properties.tex_coords.v_off[y + 1]);

			float left = properties.positions.horiz_pos[x];
			float right = properties.positions.horiz_pos[x + 1];
			float bottom = properties.positions.vert_pos[y];
			float top = properties.positions.vert_pos[y + 1];
						
			rect2 new_area = rect2(left, right, top, bottom);

			image->set_texture(texture);
			image->set_pos(new_area.pos);
			image->set_size(new_area.size);
			image->set_bounds_in_pixels(vec4(horiz_bounds, vert_bounds));
			image->centertype = Image::DEFAULT;
		} 
	}
}

void ImageMap::set_texture(Texture2D *p_texture)
{
	for (Image* image : images)
		image->set_texture(p_texture);

	texture = p_texture;
}

Texture2D* ImageMap::get_texture() const
{
	return texture;
}

void ImageMap::set_color(const Color &p_color)
{
	for (Image* image : images)
		image->set_color(p_color);

	color = p_color;
}