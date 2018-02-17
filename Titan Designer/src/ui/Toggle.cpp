#include "Toggle.h"

//=========================================================================
//Toggle
//=========================================================================

Toggle::Toggle()
{
}

Toggle::Toggle(const String& p_tex) : Toggle()
{
	tex = CONTENT->LoadTexture(p_tex);
}


Toggle::~Toggle()
{
}

void Toggle::handle_event(UIEvent * p_event)
{
	if (p_event->type == UIEvent::MOUSE_PRESS && p_event->press_type == UIEvent::DOWN)
	{
		if (area.is_in_box(p_event->pos))
		{
			selected = !selected;
			emit_signal("toggled", selected);
			update();
			return;
		}
	}
}

#include "Canvas.h"

void Toggle::notification(int p_notification)
{
	Color color;
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:
		
		if (selected)
			color = DEFAULT_THEME->get_selection_color();
		
		draw_texture(tex, area, color);

		break;

	case NOTIFICATION_TRANSLATED:
	case NOTIFICATION_RESIZED:

		update();
		break;
	}
}

vec2 Toggle::get_required_size() const
{
	return tex->get_size();
}

#undef CLASSNAME
#define CLASSNAME Toggle

void Toggle::bind_methods()
{
	REG_CSTR(0);

	REG_SIGNAL("toggled");
}

//=========================================================================
//ToggleStrip
//=========================================================================

ToggleStrip::ToggleStrip()
{
	margin = 4;
	left_margin = 4;
}

ToggleStrip::~ToggleStrip()
{
}

void ToggleStrip::handle_event(UIEvent* p_event)
{
	if (p_event->type == UIEvent::MOUSE_PRESS && p_event->press_type == UIEvent::DOWN)
	{
		for (int c = 0; c < toggles.size(); c++)
		{
			if (toggles[c].area.is_in_box(p_event->pos))
			{
				if (selected == c)
					return;

				selected = c;
				emit_signal("toggled", c);
				update();
				return;
			}
		}
	}
}

void ToggleStrip::notification(int p_notification)
{
	float offset = left_margin + area.get_left();

	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		for (int c = 0; c < toggles.size(); c++)
		{
			Color color;

			if (c == selected)
				color = DEFAULT_THEME->get_selection_color();

			vec2 tex_size = toggles[c].tex->get_size();

			toggles[c].area = rect2(offset, offset + tex_size.x, area.pos.y + tex_size.y / 2.0f, area.pos.y - tex_size.y / 2.0f);

			draw_texture(toggles[c].tex, toggles[c].area, color);
			offset += toggles[c].tex->get_size().x;
		}

		break;

	case NOTIFICATION_RESIZED:
	case NOTIFICATION_TRANSLATED:

		update();
		break;
	}
}

vec2 ToggleStrip::get_required_size() const
{
	vec2 size;

	for (int c = 0; c < toggles.size(); c++)
	{
		if (toggles[c].tex->get_size().y > size.y)
			size.y = toggles[c].tex->get_size().y;
		size.x += toggles[c].tex->get_size().x;
		size.x += margin;
	}
	return size;
}

void ToggleStrip::add_child(const String & p_tip, Ref<Texture2D> p_tex)
{
	Toggle t;
	t.tip = p_tip;
	t.tex = p_tex;
	toggles.push_back(t);
	update();
	t.tex->bind(0);
	t.tex->set_filter(Texture::BILINEAR_FILTER);
}

#undef CLASSNAME
#define CLASSNAME ToggleStrip

void ToggleStrip::bind_methods()
{
	REG_CSTR(0);

	REG_SIGNAL("toggled");
}
