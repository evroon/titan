#include "Canvas.h"

#include "input/Input.h"
#include "input/Event.h"
#include "graphics/Renderer.h"
#include "Dialog.h"
#include "ContextMenu.h"
#include "graphics/Viewport.h"

//=========================================================================
//CanvasLayer
//=========================================================================


CanvasLayer::CanvasLayer(int p_level)
{
	level = p_level;
}

CanvasLayer::~CanvasLayer()
{
}


//=========================================================================
//Canvas
//=========================================================================

Canvas::Canvas()
{
	layers.push_back(CanvasLayer(0));	// default layer

	context_menu = NULL;
	context_tip = NULL;
	dialog = NULL;

	wait_time = 500 * 1000;
	double_click_treshold = 300 * 1000;
	size = vec2(1000.0);
}

Canvas::~Canvas()
{

}

void Canvas::schedule_update(Control* p_control)
{
	scheduled_updates.push_back(p_control);
}

void Canvas::add_layer()
{
	layers.push_back(CanvasLayer(layers.size() - 1));
}

void Canvas::remove_layer(int p_index)
{
	layers.clear(p_index);
}

void Canvas::add_control(Control *p_control, int p_level)
{
	int level;

	if (p_level < layers.size())
		level = p_level;
	else
	{
		//create new layer, one above the currently highest layer
		level = layers.size();
		add_layer();
	}

	p_control->set_level(level);
	layers[level].controls.push_back(p_control);

	add_child(p_control);
	p_control->init();
}

void Canvas::remove_control(Control *p_control)
{
	layers[p_control->get_level()].controls.clear(p_control);

	remove_child(p_control);
}

void Canvas::add_control_to_top(Control * p_control)
{
	add_control(p_control, layers.size());
}

void Canvas::set_dialog(Dialog* p_dialog)
{
	dialog = p_dialog;
	add_control_to_top(p_dialog);

	focus(p_dialog);

	p_dialog->set_area(rect2(vec2(), p_dialog->get_required_size() / 2.0f));
}

void Canvas::remove_dialog()
{
	T_LOG(children.size());
	remove_control(dialog);
	T_LOG(children.size());
	dialog = NULL;
}

void Canvas::set_context_tip(const String& p_description, const vec2 &pos)
{
	if (p_description.length() == 0)
		return;

	ContextTip* tip = new ContextTip(p_description);

	if (context_tip)
		remove_context_tip();
	
	context_tip = tip;
	
	context_tip->init();
	context_tip->set_pos(pos + vec2(context_tip->get_size().x, -context_tip->get_size().y));
}

void Canvas::remove_context_tip()
{
	if (!context_tip)
		return;

	if (last_hover == context_tip)
		last_hover = NULL;

	remove_control(context_tip);

	context_tip = NULL;
	tip_time = 0;
	tip_shower = NULL;
	awaiting_tip = false;
}

void Canvas::set_context_menu(ContextMenu *p_context_menu, const vec2 &pos)
{
	if (p_context_menu->is_empty())
		return;

	if (context_menu)
		remove_context_menu();

	context_menu = p_context_menu;
	context_menu->init();
	context_menu->size_changed();
	vec2 size = context_menu->get_size();
	context_menu->set_pos(pos + vec2(size.x, -size.y));

	focus(context_menu);
}

void Canvas::remove_context_menu()
{
	if (!context_menu)
		return;

	if (context_menu->get_focused())
		focused = NULL;

	remove_control(context_menu);
	context_menu = NULL;
}

void Canvas::handle_event(Event *e)
{
	InputEvent *in = dynamic_cast<InputEvent*>(e);

	if (!in)
		return;

	vec2 mouse_pos;
	
	if (in->type != InputEvent::MOUSE_SCROLL)
		mouse_pos = in->pos;
	else
		mouse_pos = MOUSE->get_position();

	Control *hover = raycast(mouse_pos);
	
	if (in->type == InputEvent::MOUSEMOVE)
	{
		if (context_tip && tip_shower && !tip_shower->in_area(mouse_pos))
		{
			remove_context_tip();
		}

		if (MOUSE->is_pressed(Mouse::LEFT))
		{
			if (focused)
			{
				UIEvent *hover_event = new UIEvent(UIEvent::MOUSE_HOVER);
				hover_event->pos = in->pos;
				focused->handle_event(hover_event);
			}
		}
		else
		{
			if (last_hover != hover)
			{
				if (last_hover)
					last_hover->handle_event(new UIEvent(UIEvent::MOUSE_EXIT));

				if (hover)
					hover->handle_event(new UIEvent(UIEvent::MOUSE_ENTER));

				if (hover && !hover->is_of_type<ContextTip>())
				{
					tip_shower = hover;
					tip_time = TIME->get_absolutetime();
					awaiting_tip = true;
				}

				//remove_context_tip();
			}
			else if (hover)
			{
				UIEvent *hover_event = new UIEvent(UIEvent::MOUSE_HOVER);
				hover_event->pos = in->pos;
				hover->handle_event(hover_event);
			}
		}		

		last_hover = hover;
	}
	else if (in->type == InputEvent::MOUSEPRESS)
	{
		if (in->press_type == InputEvent::DOWN && in->button_type == Mouse::LEFT)
		{
			remove_context_tip(); // TODO: should be checked
			focus(hover);
		}
		
		UIEvent *click = new UIEvent(UIEvent::MOUSE_PRESS);
		click->pos = in->pos;
		click->press_type = in->press_type;
		click->button_type = in->button_type;

		if (click->press_type == InputEvent::DOWN)
		{
			if (hover == last_clicked && TIME->get_absolutetime() < click_time + double_click_treshold)
			{
				UIEvent *double_click = new UIEvent(UIEvent::MOUSE_DOUBLE_CLICK);
				click->pos = in->pos;

				if (hover)
					hover->handle_event(double_click);
				click_time = 0;
			}
			else if (hover)
				hover->handle_event(click);

			last_clicked = hover;
			click_time = TIME->get_absolutetime();
		}			
		else if (focused)
			focused->handle_event(click);
	}
	else if (in->type == InputEvent::MOUSE_SCROLL)
	{
		UIEvent *scroll = new UIEvent(UIEvent::MOUSE_SCROLL);
		scroll->scroll_type = in->scroll_type;

		if (hover)
			hover->handle_event(scroll);
	}
	else if (in->type == InputEvent::KEYPRESS)
	{
		UIEvent *press = new UIEvent(UIEvent::KEY_PRESS);
		press->key = in->key;
		press->press_type = in->press_type;
		press->mod = in->mod;

		if (focused)
			focused->handle_event(press);
	}
	else if (in->type == InputEvent::TEXT_INPUT)
	{
		UIEvent *press = new UIEvent(UIEvent::TEXT_INPUT);
		press->text = in->text;

		if (focused)
			focused->handle_event(press);
	}
}

void Canvas::init(Viewport* p_parent)
{
	parent = p_parent;
	//main_container->init(parent->renderarea);
}

void Canvas::resize()
{
	if (parent)
		size = parent->get_size();

	for (Node* child : children)
		child->cast_to_type<Control*>()->flag_size_changed();
}

void Canvas::draw()
{
	for (Node* child : children)
		child->cast_to_type<Control*>()->check_size_changed();

	if (context_tip)
		context_tip->check_size_changed();

	if (dialog)
		dialog->check_size_changed();

	if (context_menu)
		context_menu->check_size_changed();

	for (Node* child : children)
		child->cast_to_type<Control*>()->draw();

	if (context_tip)
		context_tip->draw();

	if (dialog)
		dialog->draw();

	if (context_menu)
		context_menu->draw();

	scheduled_updates.clear();
}

void Canvas::update()
{
	if (tip_shower && awaiting_tip && TIME->get_absolutetime() > tip_time + wait_time)
	{
		tip_shower->show_context_tip(MOUSE->get_position() - vec2(0, 30));
		awaiting_tip = false;
	}
}

void Canvas::free()
{
	
}

Control* Canvas::get_focused() const
{
	return focused;
}

void Canvas::focus(Control *ctrl)
{
	if (ctrl == focused)
		return;

	UIEvent *win = new UIEvent(UIEvent::FOCUS_START);
	UIEvent *lose = new UIEvent(UIEvent::FOCUS_LOSE);

	if (focused)
	{
		focused->handle_event(lose);
	}

	if (ctrl)
		ctrl->handle_event(win);

	focused = ctrl;
}

Control* Canvas::raycast(const vec2 &pos)
{
	if (context_menu && context_menu->in_area(pos))
		return context_menu;
	
	if (dialog && dialog->in_area(pos))
		return dialog->raycast(pos);

	/*for (int c = layers.size() - 1; c >= 0; c--)
	{
		CanvasLayer &l = layers[c];

		for (Control *n : l.controls)
		{
			Control* r = n->raycast(pos);

			if (r && r->get_visible())
				return r;
		}
	}*/
	for (Node *n : children)
	{
		Control* r = n->cast_to_type<Control*>()->raycast(pos);

		if (r && r->get_visible())
			return r;
	}

	return NULL;
}

Control* Canvas::raycast_layer(const CanvasLayer& l, const vec2 &pos)
{
	for (int i = 0; i < l.controls.size(); i++)
	{
		Control *r = l.controls[i]->raycast(pos);

		if (r)
			return r;
	}

	return NULL;
}

#undef CLASSNAME
#define CLASSNAME Canvas

void Canvas::bind_methods()
{
	REG_CSTR(0);
	REG_METHOD(add_control);
	REG_METHOD(remove_control);
}

//=========================================================================
//CanvasTheme
//=========================================================================

CanvasTheme::CanvasTheme()
{
	font = CONTENT->LoadFont("EngineCore/Fonts/consola.ttf", 17);
	frame = CONTENT->LoadTexture("EngineCore/UI/ContainerTemplate.png");
	highlight = CONTENT->LoadTexture("EngineCore/UI/Highlight.png");

	highlight_color = TO_RGB(100);
	selection_color = TO_RGB(vec3i(65, 133, 165));
}

Font* CanvasTheme::get_font() const
{
	return font;
}

Texture2D* CanvasTheme::get_frame() const
{
	return frame;
}

Texture2D * CanvasTheme::get_highlight() const
{
	return highlight;
}

Color CanvasTheme::get_highlight_color() const
{
	return highlight_color;
}

Color CanvasTheme::get_selection_color() const
{
	return selection_color;
}


//=========================================================================
//CanvasData
//=========================================================================

CanvasData* CanvasData::singleton;

CanvasData::CanvasData()
{
	default_shader = CONTENT->LoadShader("EngineCore/Shaders/Shader2D");

	default_theme = new CanvasTheme;
}

void CanvasData::init()
{
	singleton = new CanvasData;
}

CanvasData* CanvasData::get_singleton()
{
	return singleton;
}

Shader* CanvasData::get_default_shader() const
{
	return default_shader;
}

CanvasTheme* CanvasData::get_default_theme() const
{
	return default_theme;
}