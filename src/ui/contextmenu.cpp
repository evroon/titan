#include "contextmenu.h"

#include "ui/canvas.h"

ContextMenu::ContextMenu() {
    selection_color = Color::FromRGB(vec3i(50));
    border_color = Color::FromRGB(vec3i(150));
    background_color = Color::FromRGB(vec3i(80));
    highlight_color = Color::FromRGB(vec3i(100));

    selected = -1;
    highlighted = -1;

    left_margin = 3;
    right_margin = 3;

    font = CanvasData::get_singleton()->get_default_theme()->get_font();

    seperator_height = 5;
    option_height = font->get_height();
}

ContextMenu::~ContextMenu() {}

vec2 ContextMenu::get_required_size() const {
    float max_size_x = 0.0f;
    float size_y = 0.0f;

    for (int c = 0; c < items.size(); c++) {
        max_size_x = MAX(max_size_x, font->get_width(items[c].text));

        if (items[c].seperator)
            size_y += seperator_height;
        else
            size_y += option_height;

        size_y += 6;
    }

    return vec2(left_margin + max_size_x + right_margin, size_y);
}

void ContextMenu::handle_event(UIEvent *ui_event) {
    switch (ui_event->type) {
        case UIEvent::MOUSE_EXIT:

            highlighted = -1;
            selected = -1;
            update();

            break;

        case UIEvent::MOUSE_HOVER:

            if (!area.is_in_box(ui_event->pos)) {
                selected = -1;
                highlighted = -1;
            }

            if (selected != -1) break;

            highlighted = get_item(ui_event->pos);
            update();

            break;

        case UIEvent::MOUSE_PRESS:

            if (ui_event->button_type != Mouse::LEFT) return;

            if (ui_event->press_type == Event::DOWN) {
                selected = get_item(ui_event->pos);
                highlighted = -1;
            } else {
                if (in_area(ui_event->pos)) {
                    items[selected].signal.emit();
                    emit_signal("selected");

                    close();
                }
                selected = -1;
                highlighted = -1;
            }
            update();

            break;

        case UIEvent::FOCUS_LOSE:

            close();
            update();

            break;
    }
}

void ContextMenu::position_items() {
    float left = area.get_left();
    float right = area.get_right();
    float offset_y = area.get_top();

    for (int c = 0; c < items.size(); c++) {
        rect2 &item_area = items[c].area;

        float size;

        if (items[c].seperator)
            size = seperator_height;
        else
            size = option_height;

        size += 6;

        item_area = rect2(left, right, offset_y, offset_y - size);

        offset_y -= size;
    }
    update();
}

int ContextMenu::get_item(const vec2 &p_pos) const {
    int result = -1;

    for (int c = 0; c < items.size(); c++) {
        if (items[c].area.is_in_box(p_pos)) {
            result = c;
            break;
        }
    }

    return result;
}

void ContextMenu::notification(int p_notification) {
    switch (p_notification) {
        case NOTIFICATION_DRAW:

            draw_box(area, background_color);

            for (int c = 0; c < items.size(); c++) {
                if (items[c].seperator) {
                    vec2 start = vec2(area.get_left() + left_margin,
                                      items[c].area.pos.y);
                    vec2 end = vec2(area.get_right() - right_margin,
                                    items[c].area.pos.y);
                    draw_line(start, end, seperator_color);
                    continue;
                }
                rect2 &item_area = items[c].area;
                vec2 pos = vec2(item_area.get_left(), item_area.pos.y);

                if (selected == c)
                    draw_box(item_area, selection_color);
                else if (highlighted == c)
                    draw_box(item_area, highlight_color);

                if (items[c].icon) {
                    rect2 icon_area;
                    float tex_width = 20;

                    icon_area = rect2(
                        item_area.get_left(), item_area.get_left() + tex_width,
                        item_area.get_top(), item_area.get_bottom());
                    draw_texture(items[c].icon, icon_area, Color::White);

                    draw_text(items[c].text,
                              pos + vec2(left_margin + tex_width, 0));
                } else
                    draw_text(items[c].text, pos + vec2(left_margin, 0));
            }

            draw_highlight(area, border_color);

            break;

        case NOTIFICATION_TRANSLATED:

            position_items();

            break;

        case NOTIFICATION_RESIZED:

            position_items();

            break;
    }
}

void ContextMenu::add_item(const String &p_text,
                           const Connection &p_connection) {
    add_item(NULL, p_text, p_connection);
}

void ContextMenu::add_item(Texture2D *p_icon, const String &p_text,
                           const Connection &p_connection) {
    ContextMenuItem item;

    item.text = p_text;
    item.seperator = false;
    item.icon = p_icon;
    item.signal = Signal();

    if (p_connection.type != Connection::UNDEF)
        item.signal.attach_connection(p_connection);

    items.push_back(item);

    position_items();
    flag_size_changed();
}

void ContextMenu::add_seperator() {
    ContextMenuItem item;

    item.seperator = true;

    items.push_back(item);
    position_items();
}

void ContextMenu::set_text(int p_index, const String &p_text) {
    items[p_index].text = p_text;
    update();
}

String ContextMenu::get_text(int p_index) const { return items[p_index].text; }

int ContextMenu::get_selected() const { return selected; }

void ContextMenu::clear() {
    items.clear();
    update();
}

bool ContextMenu::is_empty() { return items.size() == 0; }

void ContextMenu::close() {
    emit_signal("closed");
    get_canvas()->remove_context_menu();
}

#undef CLASSNAME
#define CLASSNAME ContextMenu

void ContextMenu::bind_methods() {
    REG_CSTR(0);

    REG_SIGNAL("closed");
    REG_SIGNAL("selected");
}

//=========================================================================
// ContextTip
//=========================================================================

ContextTip::ContextTip() {
    background_color = TO_RGB(80);
    border_color = TO_RGB(50);

    left_margin = 4;
    right_margin = 4;
    top_margin = 4;
    bottom_margin = 4;
}

ContextTip::ContextTip(const String &p_description) : ContextTip() {
    description = p_description;
}

ContextTip::~ContextTip() {}

vec2 ContextTip::get_required_size() const {
    return vec2(
        get_default_font()->get_width(description) + left_margin + right_margin,
        get_default_font()->get_height() + top_margin + bottom_margin);
}

void ContextTip::handle_event(UIEvent *p_event) {
    switch (p_event->type) {
        case UIEvent::FOCUS_LOSE:

            close();

            break;
    }
}

void ContextTip::notification(int p_notification) {
    switch (p_notification) {
        case NOTIFICATION_DRAW:

            draw_bordered_box(area, Color::Black, background_color);

            draw_text(description,
                      vec2(area.get_left() + left_margin, area.pos.y));

            break;

        case NOTIFICATION_RESIZED:
        case NOTIFICATION_TRANSLATED:

            break;
    }
}

void ContextTip::close() { get_canvas()->remove_context_tip(); }

#undef CLASSNAME
#define CLASSNAME ContextTip

void ContextTip::bind_methods() {}
