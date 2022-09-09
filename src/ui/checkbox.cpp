#include "checkbox.h"

CheckBox::CheckBox() : CheckBox(false) {}

CheckBox::CheckBox(bool p_value) : PropertyControl(p_value) { init(); }

CheckBox::CheckBox(const Variable& p_variable) : PropertyControl(p_variable) { init(); }

void CheckBox::init() {
    tex_checked = CONTENT->LoadFontAwesomeIcon("solid/check-square");
    tex_unchecked = CONTENT->LoadFontAwesomeIcon("regular/square");
}

void CheckBox::switch_checked() { set_checked(!get_value()); }

void CheckBox::set_checked(bool p_value) {
    value_set(p_value);
    update();
}

void CheckBox::draw() {}

vec2 CheckBox::get_required_size() const { return tex_checked->get_size(); }

void CheckBox::notification(int p_notification) {
    switch (p_notification) {
        case NOTIFICATION_DRAW:

            vec2 size = tex_checked->get_size() / 2.0f;
            rect2 a = rect2(area.pos.x - size.x, area.pos.x + size.x, area.pos.y + size.y,
                            area.pos.y - size.y);

            draw_frame(a, Color::DarkGrey);

            if (get_value())
                draw_texture(tex_checked, a, Color::White);
            else
                draw_texture(tex_unchecked, a, Color::White);

            break;
    }
}

void CheckBox::handle_event(UIEvent* ui_event) {
    switch (ui_event->type) {
        case UIEvent::MOUSE_PRESS:

            if (ui_event->press_type == Event::DOWN) switch_checked();

            break;

        case UIEvent::MOUSE_ENTER:
        case UIEvent::MOUSE_EXIT:
            break;
    }
}
