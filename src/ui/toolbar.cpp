#include "toolbar.h"

#include "graphics/view.h"

//=========================================================================
// ToolBar
//=========================================================================

ToolBar::ToolBar() {
    left_margin = 14.0f;
    right_margin = 14.0f;
    seperation = 14.0f;

    font = CanvasData::get_singleton()->get_default_theme()->get_font();

    highlight_color = Color::FromRGB(vec3i(100));
    selection_color = Color::FromRGB(vec3i(65, 133, 165));

    selected = -1;
    highlighted = -1;
}

ToolBar::~ToolBar() {}

vec2 ToolBar::get_required_size() const {
    float width = left_margin + seperation * (items.size()) + right_margin;

    for (int c = 0; c < items.size(); c++) width += font->get_width(items[c].text);

    return vec2(width, 30.0f);
}

void ToolBar::notification(int p_notification) {
    switch (p_notification) {
        case NOTIFICATION_DRAW:

            draw_frame(area, TO_RGB(80));

            for (int c = 0; c < items.size(); c++) {
                rect2 item_area = items[c].area;

                rect2 highlight_area = item_area;
                highlight_area.size.y = font->get_height() / 2.0f + 3.0f;

                float text_width = font->get_width(items[c].text);
                float offset = item_area.size.x - text_width / 2.0f;

                if (selected == c)
                    draw_box(highlight_area, selection_color);
                else if (highlighted == c)
                    draw_box(highlight_area, highlight_color);

                vec2 pos = vec2(item_area.get_left() + offset, item_area.pos.y);
                draw_text(items[c].text, pos);
            }

            break;

        case NOTIFICATION_TRANSLATED:
        case NOTIFICATION_RESIZED:

            position_items();

            break;
    }
}

void ToolBar::handle_event(UIEvent* p_event) {
    switch (p_event->type) {
        case UIEvent::FOCUS_LOSE:
        case UIEvent::MOUSE_EXIT:

            highlighted = -1;
            update();
            break;

        case UIEvent::MOUSE_HOVER:

            if (!area.is_in_box(p_event->pos)) {
                selected = -1;
                highlighted = -1;
            }

            if (selected != -1) break;

            highlighted = get_item(p_event->pos);
            update();
            break;

        case UIEvent::MOUSE_PRESS:

            if (p_event->button_type != Mouse::LEFT) return;

            if (p_event->press_type == Event::DOWN) {
                selected = get_item(p_event->pos);

                open_item(selected);

                highlighted = -1;
            } else {
                selected = -1;
                highlighted = get_item(p_event->pos);
            }
            update();
            break;
    }
}

void ToolBar::position_items() {
    float offset_x = area.get_left() + left_margin;

    for (int c = 0; c < items.size(); c++) {
        float width = font->get_width(items[c].text);

        float left = offset_x;
        float right = offset_x + width;
        float top = area.get_top();
        float bottom = area.get_bottom();

        items[c].area = rect2(left, right + seperation, top, bottom);

        offset_x += width + seperation;
    }
    update();
}

int ToolBar::get_item(const vec2& p_pos) const {
    int result = -1;

    for (int c = 0; c < items.size(); c++) {
        if (items[c].area.is_in_box(p_pos)) {
            result = c;
            break;
        }
    }

    return result;
}

void ToolBar::add_item(const String& p_text) {
    ToolBarItem item;
    item.text = p_text;
    item.menu = NULL;
    items.push_back(item);

    update();
}

void ToolBar::set_menu(int p_index, ContextMenu* p_menu) {
    if (p_index < 0 || p_index > items.size()) return;

    items[p_index].menu = p_menu;
    p_menu->connect("closed", this, "menu_closed");
}

void ToolBar::open_item(int p_index) {
    if (p_index < 0 || p_index > items.size()) return;

    if (!items[p_index].menu) return;

    get_canvas()->set_context_menu(items[p_index].menu, items[p_index].area.get_bottom_left());
}

void ToolBar::menu_closed() {
    selected = -1;
    update();
}

#undef CLASSNAME
#define CLASSNAME ToolBar

void ToolBar::bind_methods() {
    REG_CSTR(0);
    REG_METHOD(menu_closed);
}
