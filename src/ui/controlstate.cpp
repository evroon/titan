#include "controlstate.h"

#include "container.h"
#include "control.h"

ControlState::ControlState(Control* p_parent) { parent = p_parent; }

ControlState::~ControlState() {}

void ControlState::handle_event(InputEvent* input_event) {
    UIEvent* key_press_event;

    switch (input_event->type) {
        case InputEvent::MOUSEMOVE:

            if (parent->in_area(vec2(input_event->pos.x, input_event->pos.y))) {
                if (!mouse_in_box) pass_mouse_enter();

                mouse_in_box = true;

                pass_mouse_hover(input_event->pos);
            } else {
                if (mouse_in_box) pass_mouse_exit();

                mouse_in_box = false;
            }
            break;

        case InputEvent::MOUSEPRESS:

            if (parent->in_area(vec2(input_event->pos.x, input_event->pos.y)))
                pass_mouse_press(input_event->pos, input_event->press_type);

            break;

        case InputEvent::MOUSE_SCROLL:

            pass_mouse_scroll(input_event->scroll_type);

            break;

        case InputEvent::KEYPRESS:

            if (!focused)
                return;
            else {
                key_press_event = new UIEvent;
                key_press_event->type = UIEvent::KEY_PRESS;
                key_press_event->key = input_event->key;
                key_press_event->mod = input_event->mod;
                key_press_event->press_type = input_event->press_type;

                parent->handle_event(key_press_event);
            }
            break;

        case InputEvent::TEXT_INPUT:

            if (!focused) return;

            key_press_event = new UIEvent;
            key_press_event->type = UIEvent::TEXT_INPUT;
            key_press_event->text = input_event->text;
            key_press_event->mod = input_event->mod;

            parent->handle_event(key_press_event);
            break;
    }
}

void ControlState::pass_mouse_hover(const vec2& pos) {
    UIEvent* e = new UIEvent;
    e->type = UIEvent::MOUSE_HOVER;
    e->pos = pos;
    parent->handle_event(e);
}
void ControlState::pass_mouse_scroll(const InputEvent::ScrollType& st) {
    UIEvent* e = new UIEvent(UIEvent::MOUSE_SCROLL);
    e->scroll_type = st;
    parent->handle_event(e);
}
void ControlState::pass_mouse_enter() {
    UIEvent* e = new UIEvent;
    e->type = UIEvent::MOUSE_ENTER;
    parent->handle_event(e);
}
void ControlState::pass_mouse_exit() {
    UIEvent* e = new UIEvent;
    e->type = UIEvent::MOUSE_EXIT;
    parent->handle_event(e);
}

void ControlState::pass_mouse_press(const vec2& pos, Event::PressType press_type) {
    UIEvent* e = new UIEvent;
    e->type = UIEvent::MOUSE_PRESS;
    e->press_type = press_type;
    e->pos = pos;
    parent->handle_event(e);
}

void ControlState::set_focused(bool value) { focused = value; }

bool ControlState::is_focused() const { return focused; }
