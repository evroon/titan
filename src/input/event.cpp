#include "event.h"

#include "eventmanager.h"
#include "world/worldobject.h"

// Event
Event::Event() { EventManager::get_manager().register_event(this); }

Event::~Event() { EventManager::get_manager().clear_event(this); }

String Event::get_name() const { return "Event"; }
String Event::to_string() const { return "Event"; }

#undef CLASSNAME
#define CLASSNAME Event

void Event::bind_methods() {
    REG_METHOD(get_name);
    REG_METHOD(to_string);
}

// InputEvent
InputEvent::InputEvent(const Type &p_type) { type = p_type; }

String InputEvent::to_string() const {
    String result = get_name() + ": ";

    switch (type) {
        case FINGERPRESS:
            result += "pos: ";
            result += pos.to_string();
            result += ", index: " + StringUtils::IntToString(index);

            result += ", presstype: ";
            result += press_type ? "UP" : "DOWN";
            break;

        case FINGERMOVE:
            result += "pos: " + pos.to_string();

            result += ", index: " + StringUtils::IntToString(index);
            break;

        case MOUSEPRESS:
            result += "pos: " + pos.to_string();

            result += ", presstype: ";
            result += press_type ? "UP" : "DOWN";

            if (button_type == Mouse::LEFT)
                result += ", buttontype: LEFT";
            else if (button_type == Mouse::CENTER)
                result += ", buttontype: CENTER";
            else if (button_type == Mouse::RIGHT)
                result += ", buttontype: RIGHT";
            break;

        case MOUSEMOVE:
            result += "pos: " + pos.to_string();
            break;

        case KEYPRESS:
            result += "key: " + key.to_string();

            result += ", presstype: ";
            result += press_type ? "UP" : "DOWN";
            break;
    }
    return result;
}
String InputEvent::get_name() const {
    switch (type) {
        case FINGERPRESS:
            return "FingerPress";

        case FINGERMOVE:
            return "FingerMove";

        case MOUSEPRESS:
            return "MousePress";

        case MOUSEMOVE:
            return "MouseMove";

        case KEYPRESS:
            return "KeyPress";

        default:
            return "null";
    }
}

void InputEvent::accept_mouse_pos(const vec2 &window_size,
                                  const vec2 &mouse_pos) {
    pos = mouse_pos - window_size / vec2(2);
    pos.y *= -1;
}

void InputEvent::accept_finger_pos(const vec2 &window_size,
                                   const vec2 &finger_pos) {
    pos = finger_pos * vec2(2) * window_size - window_size;
}

#undef CLASSNAME
#define CLASSNAME InputEvent

void InputEvent::bind_methods() {
    REG_PROPERTY(pos);

    REG_METHOD(to_string);
}

WorldObject *CollisionEvent::get_object() const { return object; }

String CollisionEvent::to_string() const {
    String result =
        "CollisionEvent: " + (contact == ContactType::BEGIN) ? "begin" : "end";

    return result;
}

#undef CLASSNAME
#define CLASSNAME CollisionEvent

void CollisionEvent::bind_methods() {
    REG_METHOD(get_object);
    REG_METHOD(to_string);
}

String UIEvent::to_string() const {
    String result = get_name() + ": ";

    switch (type) {
        case MOUSE_PRESS:
            result += "pos: " + pos.to_string();

            result += ", presstype: ";
            result += press_type ? "UP" : "DOWN";

            if (button_type == Mouse::LEFT)
                result += ", buttontype: LEFT";
            else if (button_type == Mouse::CENTER)
                result += ", buttontype: CENTER";
            else if (button_type == Mouse::RIGHT)
                result += ", buttontype: RIGHT";
            break;
    }
    return result;
}

#undef CLASSNAME
#define CLASSNAME UIEvent

void UIEvent::bind_methods() { REG_METHOD(to_string); }
