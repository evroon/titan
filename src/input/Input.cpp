#include "Input.h"

#include "Application.h"
#include "WindowManager.h"

Input *Input::singleton;

Input::Input()
{

}

void Input::init()
{
	singleton = new Input;
}

void Input::HandleEvent(const SDL_Event &event)
{
	vec2i sizei = parent->window->size;
	vec2 size = vec2(to_float(sizei.x), to_float(sizei.y));
	vec2 button_pos = vec2(to_float(event.button.x), to_float(event.button.y));

	if (event.type == SDL_FINGERDOWN || event.type == SDL_FINGERUP)
	{
		InputEvent::PressType pt;

		if (event.type == SDL_FINGERDOWN)
			pt = InputEvent::DOWN;
		else if (event.type == SDL_FINGERUP)
			pt = InputEvent::UP;

		InputEvent *e = new InputEvent(InputEvent::FINGERPRESS);
		e->accept_finger_pos(size, button_pos);
		e->press_type = pt;
		e->index = (int)event.tfinger.fingerId;

		AddEvent(e);
	}
	else if (event.type == SDL_FINGERMOTION)
	{
		InputEvent *e = new InputEvent(InputEvent::FINGERMOVE);
		e->accept_finger_pos(size, button_pos);
		e->index = (int)event.tfinger.fingerId;

		AddEvent(e);
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		InputEvent::PressType pt;

		if (event.type == SDL_MOUSEBUTTONDOWN)
			pt = InputEvent::DOWN;
		else if (event.type == SDL_MOUSEBUTTONUP)
			pt = InputEvent::UP;

		InputEvent *e = new InputEvent(InputEvent::MOUSEPRESS);
		e->accept_mouse_pos(size, button_pos);
		e->button_type = MOUSE->get_ButtonType(event.button.button);
		e->press_type = pt;

		AddEvent(e);
	}
	else if (event.type == SDL_MOUSEMOTION)
	{
		InputEvent *e = new InputEvent(InputEvent::MOUSEMOVE);
		e->accept_mouse_pos(size, vec2(event.motion.x, event.motion.y));

		AddEvent(e);
	}
	else if (event.type == SDL_MOUSEWHEEL)
	{
		InputEvent *e = new InputEvent(InputEvent::MOUSE_SCROLL);
		
		if (event.wheel.x > 0)
			e->scroll_type = Event::SCROLL_RIGHT;
		else if (event.wheel.x < 0)
			e->scroll_type = Event::SCROLL_LEFT;
		else if (event.wheel.y > 0)
			e->scroll_type = Event::SCROLL_UP;
		else
			e->scroll_type = Event::SCROLL_DOWN;

		AddEvent(e);
	}
	else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	{
		//if (event.key.repeat)
		//	return;

		InputEvent::PressType pt;

		if (event.type == SDL_KEYDOWN)
			pt = InputEvent::DOWN;
		else if (event.type == SDL_KEYUP)
			pt = InputEvent::UP;

		InputEvent *e = new InputEvent(InputEvent::KEYPRESS);
		e->key = Key(event.key.keysym.sym);
		e->mod = ModKey(event.key.keysym.mod);
		e->press_type = pt;
		e->repeat = event.key.repeat != 0;

		AddEvent(e);
	}
	else if (event.type == SDL_TEXTINPUT)
	{
		InputEvent *e = new InputEvent(InputEvent::TEXT_INPUT);
		e->key = Key(event.key.keysym.sym);
		e->mod = ModKey(event.key.keysym.mod);
		e->text = event.text.text;
		AddEvent(e);
	}
	else if (event.type == SDL_DROPFILE)
	{
		AddEvent(new DropEvent(event.drop.file));
	}
}

void Input::AddEvent(Event *e)
{
	events.push_back(e);
}

void Input::Clean()
{
	events.clean();
}

void Input::enable_text_input()
{
	SDL_StartTextInput();
}

void Input::disable_text_input()
{
	SDL_StopTextInput();
}

void Input::Free()
{
	events.clean();
}

#undef CLASSNAME
#define CLASSNAME Input

void Input::bind_methods()
{
	REG_SINGLETON(singleton);
}