#pragma once

#include "Key.h"
#include "Math.h"
#include "utility/StringUtils.h"
#include "physics/RigidBody.h"
#include "Mouse.h"

class Event : public Object
{
	OBJ_DEFINITION(Event, Object)

public:
	Event();
	~Event();

	enum PressType
	{
		DOWN,
		UP
	};

	enum ScrollType
	{
		SCROLL_UP,
		SCROLL_DOWN,
		SCROLL_LEFT,
		SCROLL_RIGHT
	};

	virtual String get_name() const;
	virtual String to_string() const;

	static void bind_methods();
};

class InputEvent : public Event
{
	OBJ_DEFINITION(InputEvent, Event)

public:
	enum Type
	{
		FINGERPRESS,
		FINGERMOVE,
		MOUSEPRESS,
		MOUSEMOVE,
		MOUSE_SCROLL,
		KEYPRESS,
		TEXT_INPUT
	};

	InputEvent(const Type &p_type);

	String to_string() const override;
	String get_name() const override;

	void accept_mouse_pos(const vec2 &window_size, const vec2 &mouse_pos);
	void accept_finger_pos(const vec2 &window_size, const vec2 &finger_pos);

	void set_pos(vec2 p_pos) { pos = p_pos; }
	vec2 get_pos() const { return pos; }

	static void bind_methods();

	//Event Data
	Key key;
	ModKey mod;
	vec2 pos;
	Type type;
	int index;
	PressType press_type;
	Mouse::ButtonType button_type;
	ScrollType scroll_type;
	String text;
	bool repeat;
};

class CollisionEvent : public Event
{
	OBJ_DEFINITION(CollisionEvent, Event)

public:
	CollisionEvent() { }
	
	enum ContactType
	{
		BEGIN,
		END
	};

	ContactType contact;

	RigidBody *object;
};

class UIEvent : public Event
{
	OBJ_DEFINITION(UIEvent, Event)

public:
	enum Type
	{
		// mouse events
		MOUSE_HOVER,
		MOUSE_EXIT,
		MOUSE_ENTER,
		MOUSE_PRESS,
		MOUSE_SCROLL,
		MOUSE_DOUBLE_CLICK,

		// focus events
		FOCUS_START,
		FOCUS_LOSE,

		// text input events
		TEXT_INPUT,

		// key event
		KEY_PRESS,

		DROP_FILE,
		DROP_STRING
	};

	UIEvent() { }
	UIEvent(Type p_type) { type = p_type; }
	~UIEvent() {}
	
	// event data
	Type type;

	// mouse, finger and key data
	vec2 pos;
	int index;
	PressType press_type;
	Mouse::ButtonType button_type;
	ScrollType scroll_type;
	Key key;
	ModKey mod;

	// text data
	String text;
	int start, length;
};

class DropEvent : public Event
{
	OBJ_DEFINITION(DropEvent, Event)

public:
	DropEvent(const String &name)
	{
		filename = name;
	}
	String filename;
};

class DrawEvent : public Event
{
	//unimplemented
};

class UpdateEvent : public Event
{
	//unimplemented
};