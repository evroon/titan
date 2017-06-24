#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <cassert>

#include <GL\glew.h>
#include <GL\freeglut.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "math/Vec3.h"

#include "Resource.h"

struct aiTexture;

class Texture : public Resource
{
	OBJ_DEFINITION(Texture, Resource)

public:
	Texture() = default;
	Texture(int p_t) { type = p_t; }
	~Texture();

	void Bind(int p_unit);
	void UnBind(int p_unit);
	GLuint ID;

protected:
	void GenerateGLTexture();

private:
	bool loaded = false;
	int type;
};

class Texture3D : public Texture
{
	OBJ_DEFINITION(Texture3D, Texture)

public:
	Texture3D() : Texture(GL_TEXTURE_3D) { GenerateGLTexture(); } //TODO

	vec3i size;
};

class Texture2D : public Texture
{
	OBJ_DEFINITION(Texture2D, Texture)

public:
	Texture2D() = default;
	Texture2D(const vec2& p_size);
	Texture2D(const vec2i& p_size) : Texture2D(vec2(to_float(p_size.x), to_float(p_size.y))) { }
	Texture2D(const String &p_filepath);
	Texture2D(SDL_Surface *p_surface);

	Texture2D(aiTexture* p_texture);

	vec2 size;
};

class Texture1D : public Texture
{
	OBJ_DEFINITION(Texture1D, Texture)

public:
	Texture1D() = default;
	Texture1D(int p_size);

	int size;
};

struct SDL_Surface;

class RawTexture2D : public Texture2D
{
	OBJ_DEFINITION(RawTexture2D, Texture2D);

public:
	RawTexture2D(const String &p_filepath);
	RawTexture2D(SDL_Surface *p_surface);

	void free();

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