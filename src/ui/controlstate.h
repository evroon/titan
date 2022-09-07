#pragma once

#include "input/event.h"

class Control;

class ControlState
{
public:
	ControlState(Control *p_parent);
	~ControlState();

	void handle_event(InputEvent *e);

	void set_focused(bool value);
	bool is_focused() const;

private:
	void pass_mouse_hover(const vec2 &pos);
	void pass_mouse_scroll(const InputEvent::ScrollType & st);
	void pass_mouse_enter();
	void pass_mouse_exit();

	void pass_mouse_press(const vec2 &pos, Event::PressType press_type);

	Control *parent;

	bool mouse_in_box;
	bool focused;
};