#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <cassert>

#include "core/definitions.h"

#if PLATFORM == LINUX
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <GL/glew.h>
#include <GL/glu.h>
#else
#include "sdl.h"
#include "sdl_image.h"
#include "sdl_ttf.h"
#include <GL\glew.h>
#include <GL\freeglut.h>
#endif

#include "math/vec3.h"

#include "resource.h"

struct aiTexture;

class Texture : public Resource
{
	OBJ_DEFINITION(Texture, Resource)

public:
	Texture() = default;
	Texture(int p_t) { type = p_t; }
	virtual ~Texture();

	enum FilterType
	{
		NO_FILTER,
		BILINEAR_FILTER,
		TRILINEAR_FILTER,
	};

	void bind(int p_unit);
	void unbind(int p_unit);

	void set_filter(FilterType p_filter_type);
	void set_shadow_parameters();

	unsigned get_id() const;

	static void bind_methods();

protected:
	void generate_gl_texture();

	bool loaded = false;
	GLuint id;
	FilterType filter_type;
	int type;
};

class Texture3D : public Texture
{
	OBJ_DEFINITION(Texture3D, Texture)

public:
	Texture3D() : Texture(GL_TEXTURE_3D) { }
	Texture3D(const vec3& p_size, int p_index);

	vec3 get_size() const;

	static void bind_methods();

private:
	vec3 size;
};

class Texture2D : public Texture
{
	OBJ_DEFINITION(Texture2D, Texture)

public:
	Texture2D() : Texture(GL_TEXTURE_2D) { }
	Texture2D(const vec2& p_size, bool p_byte);
	Texture2D(const vec2i& p_size, bool p_byte = true) : Texture2D(vec2(to_float(p_size.x), to_float(p_size.y)), p_byte) { }
	Texture2D(const vec2& p_size, int p_index);
	Texture2D(const String &p_filepath);
	Texture2D(const String& p_filepath, const vec2i& p_size, const Color& p_color);
	Texture2D(SDL_Surface *p_surface);
	Texture2D(aiTexture* p_texture);

	vec2 get_size() const;

	static void bind_methods();

protected:
	vec2 size;
};

class Texture1D : public Texture
{
	OBJ_DEFINITION(Texture1D, Texture)

public:
	Texture1D() = default;
	Texture1D(int p_size);

	static void bind_methods();

protected:
	int size;
};

struct SDL_Surface;

class RawTexture2D : public Texture2D
{
	OBJ_DEFINITION(RawTexture2D, Texture2D);

public:
	RawTexture2D(const String &p_filepath);
	RawTexture2D(SDL_Surface *p_surface);

	void free() override;

	Color read_pixel(const vec2i& p_pos);

private:
	SDL_Surface *surface;
};

class DepthTexture2D : public Texture2D
{
	OBJ_DEFINITION(DepthTexture2D, Texture2D);

public:
	DepthTexture2D(const vec2i& p_size);
};