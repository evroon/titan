#include "font.h"

#include <functional>

#if PLATFORM == LINUX
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#else
#include "sdl.h"
#include "sdl_ttf.h"
#endif

#include "core/contentmanager.h"

#include "core/string.h"
#include "core/tchar.h"

#include "math/arraymath.h"

Font::Font()
{
}

Font::Font(const String &name, int size) : Font()
{
	set_file(name);
	font = TTF_OpenFont(name.c_str(), size);

	if ((String)TTF_GetError() != "") {
		T_ERROR("Could not open font: " + std::string(TTF_GetError()));
		return;
	}

	int w = 0, h = 0;

	if (TTF_SizeText(font, " ", &w, &h))
		T_ERROR(TTF_GetError());

	height = to_float(h);

	renderer = new FontRenderer(this);
}

Font::~Font()
{
	TTF_CloseFont(font);
	font = NULL;
}

void Font::Init()
{
	if (TTF_Init())
		T_ERROR("Could not initialize TTF:" + std::string(TTF_GetError()));
}

void Font::Quit()
{
	TTF_Quit();
}

Texture2D* Font::DrawToTex(const String& src)
{
	String s = src;

	if (s.size() == 0  || src == "\t" || src == "\n")
		return DrawToTex(" ");

	SDL_Color color = { 255, 255, 255 };
	text = TTF_RenderText_Blended(font, src.c_str(), color);

	if (!text)
		T_ERROR("Could not render text: " + std::string(TTF_GetError()));

	Texture2D *tex = new Texture2D(text);
	tex->set_filter(Texture2D::BILINEAR_FILTER);
	CONTENT->AddTexture(tex);
	return tex;
}

int Font::get_index(const String& text, float p_offset) const
{
	if (p_offset < 0)
		return 0;

	Array<float> offsets;

	for (int c = 0; c < text.size() + 1; c++)
		offsets.push_back(get_width(text.substr(0, c)));

	return to_int(ArrayMath::closest_to_index(p_offset, offsets));
}

float Font::get_width(const String& text) const
{
	int temp = 0;

	if (TTF_SizeText(font, text.c_str(), &temp, NULL))
	{
		T_ERROR(TTF_GetError());
		return 0.0f;
	}

	int tab_count = text.count('\t');
	float w = to_float(temp);

	if (tab_count > 0)
		w += get_width(" ") * 2.0f * to_float(tab_count);

	return to_float(w);
}

float Font::get_height() const
{
	return height;
}

FontRenderer* Font::get_renderer() const
{
	return renderer;
}

#undef CLASSNAME
#define CLASSNAME Font

void Font::bind_methods()
{
	//test
	//REG_PROPERTY(height);

	REG_METHOD(get_index);
}

//=========================================================================
//FontRenderer
//=========================================================================

FontRenderer::FontRenderer(Font* p_font)
{
	font = p_font;

	String text;

	for (int i = 33; i < 127; i++)
		text += i;

	texture = font->DrawToTex(text);

	init();
}

Texture2D* FontRenderer::get_texture() const
{
	return texture;
}

void FontRenderer::init()
{
	String text;

	for (int i = 33; i < 127; i++)
	{
		starts[i - 33] = font->get_width(text);

		text += i;

		ends[i - 33] = font->get_width(text);
	}

	space_offset = font->get_width(" ");

	//font->get_width(" ") == font->get_width("\t") for some reason
	tab_offset = font->get_width(" ") * 3.0f;
}

vec4 FontRenderer::get_bounds(Char p_c) const
{
	return vec4(starts[p_c.kar - 33], ends[p_c.kar - 33], texture->get_size().y, 0);
}

float FontRenderer::get_space_offset() const
{
	return space_offset;
}

float FontRenderer::get_tab_offset() const
{
	return tab_offset;
}
