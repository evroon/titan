#include "texture.h"

#include "assimp/texture.h"
#include "graphics/renderer.h"

//=========================================================================
// Texture
//=========================================================================

void Texture::generate_gl_texture() {
    glGenTextures(1, &id);
    glBindTexture(type, id);
    set_filter(NO_FILTER);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    loaded = true;
}

Texture::~Texture() { glDeleteTextures(1, &id); }

void Texture::bind(int p_unit) {
    if (!loaded) return;

    glActiveTexture(GL_TEXTURE0 + p_unit);
    glBindTexture(type, id);
}
void Texture::unbind(int p_unit) {
    if (!loaded) return;

    glActiveTexture(GL_TEXTURE0 + p_unit);
    glBindTexture(type, 0);
}

void Texture::set_filter(FilterType p_filter_type) {
    filter_type = p_filter_type;

    switch (filter_type) {
        case NO_FILTER:

            glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;

        case BILINEAR_FILTER:

            glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;

        case TRILINEAR_FILTER:

            glTexParameteri(type, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glGenerateMipmap(type);
            break;
    }
}

void Texture::set_shadow_parameters() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
                    GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

unsigned Texture::get_id() const { return id; }

#undef CLASSNAME
#define CLASSNAME Texture

void Texture::bind_methods() {}

//=========================================================================
// Texture3D
//=========================================================================

Texture3D::Texture3D(const vec3 &p_size, int p_index) : Texture3D() {
    size = p_size;
    id = p_index;
    loaded = true;
}

vec3 Texture3D::get_size() const { return size; }

void Texture3D::bind_methods() {}

//=========================================================================
// Texture2D
//=========================================================================

Texture2D::Texture2D(const vec2 &p_size, int p_index) : Texture2D() {
    loaded = true;
    size = p_size;
    id = p_index;
}

Texture2D::Texture2D(const vec2 &p_size, bool p_byte = true) : Texture2D() {
    generate_gl_texture();

    if (p_byte)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)p_size.x,
                     (GLsizei)p_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)p_size.x,
                     (GLsizei)p_size.y, 0, GL_RGBA, GL_FLOAT, 0);

    size = p_size;
}

Texture2D::Texture2D(const String &p_filepath) : Texture2D() {
    String path = File(p_filepath).get_absolute_path();
    SDL_Surface *image = IMG_Load(path.c_str());

    if (!image) {
        T_ERROR("Failed to load Image: " + path +
                ", reason: " + IMG_GetError());
        return;
    }

    generate_gl_texture();

    int texture_format = image->format->Amask ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, texture_format, image->w, image->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, image->pixels);

    size = vec2(to_float(image->w), to_float(image->h));

    SDL_FreeSurface(image);
}

Texture2D::Texture2D(const String &p_filepath, const vec2i &p_size,
                     const Color &p_color)
    : Texture2D() {
    SDL_Surface *image = IMG_Load((p_filepath).c_str());

    if (!image) {
        T_ERROR("Failed to load svg: " + p_filepath +
                ", reason: " + IMG_GetError());
        return;
    }

    // Keep aspect ratio.
    float scaling = MIN(to_float(p_size.x) / to_float(image->w),
                        to_float(p_size.y) / to_float(image->h));
    vec2i rescaled_size = vec2i(image->w * scaling, image->h * scaling);

    SDL_Rect dst = {0, 0, rescaled_size.x, rescaled_size.y};
    SDL_Surface *processed =
        SDL_CreateRGBSurface(image->flags, rescaled_size.x, rescaled_size.y, 32,
                             image->format->Rmask, image->format->Gmask,
                             image->format->Bmask, image->format->Amask);
    SDL_UpperBlitScaled(image, NULL, processed, &dst);

    if (!processed) {
        T_ERROR("Failed to resize svg: " + p_filepath +
                ", reason: " + SDL_GetError());
        return;
    }

    unsigned char *pixels = (unsigned char *)processed->pixels;

    for (int x = 0; x < processed->w; x++) {
        for (int y = 0; y < processed->h; y++) {
            pixels[4 * (y * processed->w + x) + 0] = p_color.r * 255;
            pixels[4 * (y * processed->w + x) + 1] = p_color.g * 255;
            pixels[4 * (y * processed->w + x) + 2] = p_color.b * 255;
        }
    }

    generate_gl_texture();

    int texture_format = processed->format->Amask ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, texture_format, processed->w, processed->h,
                 0, texture_format, GL_UNSIGNED_BYTE, processed->pixels);
    set_filter(BILINEAR_FILTER);

    size = vec2(to_float(processed->w), to_float(processed->h));

    SDL_FreeSurface(image);
    SDL_FreeSurface(processed);
}

Texture2D::Texture2D(SDL_Surface *p_surface) : Texture2D() {
    generate_gl_texture();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int texture_format = p_surface->format->Amask ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, texture_format, p_surface->w, p_surface->h,
                 0, texture_format, GL_UNSIGNED_BYTE, p_surface->pixels);

    size = vec2(to_float(p_surface->w), to_float(p_surface->h));

    SDL_FreeSurface(p_surface);
}

Texture2D::Texture2D(aiTexture *p_texture) : Texture2D() {
    generate_gl_texture();

    int texture_format = GL_BGRA;
    glTexImage2D(GL_TEXTURE_2D, 0, texture_format, (int)p_texture->mWidth,
                 (int)p_texture->mHeight, 0, texture_format,
                 GL_UNSIGNED_INT_8_8_8_8_REV, p_texture->pcData);

    size = vec2(to_float(p_texture->mWidth), to_float(p_texture->mHeight));
}

vec2 Texture2D::get_size() const { return size; }

#undef CLASSNAME
#define CLASSNAME Texture2D

void Texture2D::bind_methods() { REG_CSTR(0); }

//=========================================================================
// Texture1D
//=========================================================================

Texture1D::Texture1D(int p_size) : Texture(GL_TEXTURE_1D) {
    generate_gl_texture();

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, p_size, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, 0);
}

void Texture1D::bind_methods() {}

//=========================================================================
// RawTexture2D
//=========================================================================

RawTexture2D::RawTexture2D(const String &p_filepath) {
    surface = IMG_Load((p_filepath).c_str());

    if (!surface) {
        T_ERROR("Failed to load Image: " + p_filepath);
        return;
    }

    size = vec2(to_float(surface->w), to_float(surface->h));
}

RawTexture2D::RawTexture2D(SDL_Surface *p_surface) { surface = p_surface; }

void RawTexture2D::free() { SDL_FreeSurface(surface); }

Color RawTexture2D::read_pixel(const vec2i &p_pos) {
    Uint32 *pixels = (Uint32 *)surface->pixels;
    Uint32 pixel = pixels[p_pos.y * surface->pitch / 4 + p_pos.x];
    Uint8 r, g, b;

    SDL_GetRGB(pixel, surface->format, &r, &g, &b);

    return Color(r, g, b);
}

//=========================================================================
// DepthTexture2D
//=========================================================================

DepthTexture2D::DepthTexture2D(const vec2i &p_size) {
    size = vec2(to_float(p_size.x), to_float(p_size.y));
    loaded = true;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, to_int(p_size.x),
                 to_int(p_size.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
}
