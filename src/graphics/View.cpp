#include "View.h"

#include "world/Sprite.h"
#include "core/titanscript/TitanScript.h"

View View::singleton;

View::View()
{
	Viewport *default_viewport = new Viewport;

	bind_viewport(default_viewport);
	set_default_viewport(default_viewport);
	set_active_viewport(default_viewport);

	default_viewport->set_canvas(new Canvas);
}

View::~View()
{

}

void View::init(const vec2& p_default_size)
{
	default_viewport->resize(p_default_size);
}

void View::draw()
{
	default_viewport->draw();

	/*for (int c = 0; c < viewports.size(); c++)
		viewports[c]->draw();*/
}

void View::update()
{
	for (int c = 0; c < viewports.size(); c++)
		viewports[c]->update();
}

void View::handle_event(Event *event)
{
	for (int c = 0; c < viewports.size(); c++)
		viewports[c]->handle_event(event);
}

void View::bind_viewport(Viewport *viewport)
{
	viewport->bind_parent(this);
	viewports.push_back(viewport);
}

void View::free()
{

}

void View::set_default_viewport(Viewport *p_viewport)
{
	default_viewport = p_viewport;
}

Viewport* View::get_default_viewport() const
{
	return default_viewport;
}

void View::set_active_viewport(Viewport* p_viewport)
{
	active_viewport = p_viewport;
}

Viewport* View::get_active_viewport() const
{
	return active_viewport;
}

View* View::get_singleton()
{
	return &singleton;
}

#undef CLASSNAME
#define CLASSNAME View

void View::bind_methods()
{
	REG_PROPERTY(default_viewport);
}