#include "frame.h"

#include "core/contentmanager.h"

Frame::Frame() {
    set_default_texture();

    set_color(color);

    add_child(&image_map);
}

Frame::~Frame() {}

void Frame::notification(int p_notification) {
    switch (p_notification) {
        case NOTIFICATION_DRAW:

            image_map.draw();

            break;

        case NOTIFICATION_RESIZED:

            build();

            break;
    }
}

void Frame::build() {
    vec2 tex_size = texture->get_size();
    vec2 size = area.size;

    vec2 pos = area.get_bottom_left();
    vec2 extension = {size.x * 2 - tex_size.x * 2 / 3,
                      size.y * 2 - tex_size.x * 2 / 3};

    Array<float> h_off = {0, tex_size.x / 3, 2 * tex_size.x / 3, tex_size.x};
    Array<float> v_off = {0, tex_size.y / 3, 2 * tex_size.y / 3, tex_size.y};
    Array<float> h_pos = {pos.x, pos.x + h_off[1],
                          pos.x + extension.x + h_off[1],
                          pos.x + extension.x + h_off[2]};
    Array<float> v_pos = {pos.y, pos.y + v_off[1],
                          pos.y + extension.y + v_off[1],
                          pos.y + extension.y + v_off[2]};

    SplitProperties::TexCoords texc = {h_off, v_off};
    SplitProperties::Positions positions = {h_pos, v_pos};

    image_map.build(texture, texc, positions);
}

void Frame::set_texture(Texture2D *p_texture) {
    texture = p_texture;
    image_map.set_texture(texture);
}

void Frame::set_default_texture() {
    set_texture(CONTENT->LoadTexture("engine/ui/Container.png"));
}

void Frame::set_color(const Color &p_color) {
    color = p_color;
    image_map.set_color(color);
}

//=========================================================================
// Highlighter
//=========================================================================

Highlighter::Highlighter() {
    set_texture(CONTENT->LoadTexture("engine/ui/Highlight.png"));
    set_color(Color::FromRGB(vec3i(0, 110, 250)));
}
