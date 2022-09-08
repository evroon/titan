#include "textbutton.h"

#include "canvas.h"

TextButton::TextButton() : TextButton("") {}

TextButton::TextButton(const String &p_text) {
    font = CanvasData::get_singleton()->get_default_theme()->get_font();

    text = p_text;
    left_margin = right_margin = 5;
}

vec2 TextButton::get_required_size() const {
    return vec2(font->get_width(text) + left_margin + right_margin,
                font->get_height());
}

void TextButton::notification(int p_notification) {
    switch (p_notification) {
        case NOTIFICATION_DRAW:

            draw_frame(area, color);
            draw_text(text, vec2(area.pos.x - font->get_width(text) / 2.0f,
                                 area.pos.y));

            break;

        case NOTIFICATION_RESIZED:
        case NOTIFICATION_TRANSLATED:

            update();
            break;
    }
}

#undef CLASSNAME
#define CLASSNAME TextButton

void TextButton::set_text(const String &p_text) {
    text = p_text;
    update();
}

String TextButton::get_text() const { return text; }

void TextButton::bind_methods() {
    REG_CSTR(0);
    REG_CSTR_OVRLD_1(String);
    REG_PROPERTY(text);
}
