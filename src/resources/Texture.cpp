#include "Texture.h"

#include "assimp/texture.h"
#include "graphics/Renderer.h"

//=========================================================================
//Texture
//=========================================================================

void Texture::generate_gl_texture()
{
	glGenTextures(1, &id);
	glBindTexture(type, id);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
	loaded = true;
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::bind(int p_unit)
{
	if (!loaded)
		return;

    glActiveTexture(GL_TEXTURE0 + p_unit);
    glBindTexture(type, id);
}
void Texture::unbind(int p_unit)
{
	if (!loaded)
		return;

	glActiveTexture(GL_TEXTURE0 + p_unit);
	glBindTexture(type, 0);
}

unsigned Texture::get_id()
{
	return id;
}

//=========================================================================
//Texture3D
//=========================================================================

Texture3D::Texture3D(const vec3& p_size, int p_index) : Texture3D()
{
	size = p_size;
	id = p_index;
	loaded = true;
}


vec3 Texture3D::get_size() const
{
	return size;
}

//=========================================================================
//Texture2D
//=========================================================================

Texture2D::Texture2D(const vec2& p_size, int p_index) : Texture2D()
{
	loaded = true;
	size = p_size;
	id = p_index;
}

Texture2D::Texture2D(const vec2& p_size, bool p_byte = true) : Texture2D()
{ 
	generate_gl_texture();

	if (p_byte)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)p_size.x, (GLsizei)p_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei)p_size.x, (GLsizei)p_size.y, 0, GL_RGBA, GL_FLOAT, 0);

	size = p_size;
}

Texture2D::Texture2D(const String &p_filepath) : Texture2D()
{
	SDL_Surface *image = IMG_Load((p_filepath).c_str());

	if (!image)
	{
		T_ERROR("Failed to load Image: " + p_filepath);
		return;
	}

	generate_gl_texture();

	int texture_format = image->format->Amask ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, texture_format, image->w, image->h, 0, texture_format, GL_UNSIGNED_BYTE, image->pixels);

	size = vec2(to_float(image->w), to_float(image->h));

	SDL_FreeSurface(image);
}

Texture2D::Texture2D(SDL_Surface *p_surface) : Texture2D()
{
	generate_gl_texture();

	int texture_format = p_surface->format->Amask ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, texture_format, p_surface->w, p_surface->h, 0, texture_format, GL_UNSIGNED_BYTE, p_surface->pixels);

	size = vec2(to_float(p_surface->w), to_float(p_surface->h));

	SDL_FreeSurface(p_surface);
}

Texture2D::Texture2D(aiTexture* p_texture) : Texture2D()
{
	generate_gl_texture();

	int texture_format = GL_BGRA;
	glTexImage2D(GL_TEXTURE_2D, 0, texture_format, (int) p_texture->mWidth, (int) p_texture->mHeight, 0, texture_format, GL_UNSIGNED_INT_8_8_8_8_REV, p_texture->pcData);

	size = vec2(to_float(p_texture->mWidth), to_float(p_texture->mHeight));
}

vec2 Texture2D::get_size() const
{
	return size;
}

//=========================================================================
//Texture1D
//=========================================================================

Texture1D::Texture1D(int p_size) : Texture(GL_TEXTURE_1D)
{
	generate_gl_texture();

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, p_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
}

//=========================================================================
//RawTexture2D
//=========================================================================

RawTexture2D::RawTexture2D(const String& p_filepath)
{
	surface = IMG_Load((p_filepath).c_str());

	if (!surface)
	{
		T_ERROR("Failed to load Image: " + p_filepath);
		return;
	}

	size = vec2(to_float(surface->w), to_float(surface->h));
}

RawTexture2D::RawTexture2D(SDL_Surface * p_surface)
{
	surface = p_surface;
}

void RawTexture2D::free()
{
	SDL_FreeSurface(surface);
}

Color RawTexture2D::read_pixel(const vec2i & p_pos)
{
	Uint32* pixels = (Uint32*)surface->pixels;

	Uint32 pixel = pixels[p_pos.y * surface->pitch / 4 + p_pos.x];

	Uint8 r, g, b;

	SDL_GetRGB(pixel, surface->format, &r, &g, &b);

	return Color(r, g, b);
}

//=========================================================================
//DepthTexture2D
//=========================================================================

DepthTexture2D::DepthTexture2D(const vec2i& p_size)
{
	size = vec2(p_size.x, p_size.y);
	loaded = true;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, to_int(p_size.x), to_int(p_size.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
}