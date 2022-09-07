#pragma once

#include "texture.h"
#include "world/sprite.h"

class FontRenderer;

class Font : public Resource
{
	OBJ_DEFINITION(Font, Resource)

public:
	Font();
	Font(const String &name, int size);
	~Font();

	static void Init();
	static void Quit();

	Texture2D* DrawToTex(const String &src);
	int get_index(const String &text, float p_offset) const;
	float get_width(const String &text) const;
	float get_height() const;

	FontRenderer* get_renderer() const;

	static void bind_methods();

	float height;

private:
	SDL_Surface *text;
	TTF_Font *font;
	FontRenderer* renderer;
};

class FontRenderer
{
public:
	FontRenderer(Font* p_font);

	Texture2D* get_texture() const;
	vec4 get_bounds(Char p_c) const;

	float get_space_offset() const;
	float get_tab_offset() const;


	void init();

private:
	Font* font;
	Texture2D* texture;

	float space_offset, tab_offset;

	float starts[94];
	float ends[94];
};