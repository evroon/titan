#include "imagebutton.h"

ImageButton::ImageButton(const String& tex_name)
    : ImageButton(CONTENT->LoadTexture(tex_name)) {}

ImageButton::ImageButton(Texture2D* p_texture) {
    texture = p_texture;
    texture->bind(0);
    texture->set_filter(Texture::BILINEAR_FILTER);

    for (int c = 0; c < 4; c++) texture_margins[c] = 5;
}

void ImageButton::notification(int p_notification) {
    switch (p_notification) {
        case NOTIFICATION_DRAW:

            if (highlighted || selected) draw_frame(area, color);

            draw_texture(texture, rect2(area.pos, texture->get_size() / 2.0f),
                         Color::White);

            break;

        case NOTIFICATION_RESIZED:
        case NOTIFICATION_TRANSLATED:

            update();
            break;
    }
}

void ImageButton::set_icon(const String& p_icon) {
    set_image(CONTENT->LoadFontAwesomeIcon(p_icon));
}

void ImageButton::set_image(Texture2D* p_texture) {
    texture = p_texture;
    update();
}

Texture2D* ImageButton::get_image() const { return texture; }

vec2 ImageButton::get_required_size() const {
    return texture->get_size() + vec2(texture_margins[0] + texture_margins[2],
                                      texture_margins[1] + texture_margins[3]);
}
