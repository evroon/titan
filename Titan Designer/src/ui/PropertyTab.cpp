#include "PropertyTab.h"

#include "Dock.h"
#include "resources/Font.h"

#include "ImageButton.h"
#include "TextField.h"

PropertyTab::PropertyTab(Object* p_object)
{
	property_view = new PropertyView;
	add_child(property_view);

	property_view->set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	property_view->set_margins(4, 4, 4, 40);

	name_field = new TextField;
	back_button = new IconButton("solid/arrow-left");
	forward_button = new IconButton("solid/arrow-right");
	down_button = new IconButton("solid/arrow-down");
	bar = new Container;

	//back_button
	back_button->connect("clicked", this, "go_back");
	back_button->set_tip_description("Back");
	
	//forward_button
	forward_button->connect("clicked", this, "go_forward");
	forward_button->set_tip_description("Forward");

	//down_button
	down_button->connect("clicked", this, "show_history");
	down_button->set_tip_description("Forward");

	//name_field
	name_field->set_anchors(ANCHOR_BEGIN, ANCHOR_END, ANCHOR_BEGIN, ANCHOR_END);
	name_field->set_margins(4, 36, 196, 4);

	//buttons
	bar->add_child(back_button);
	bar->add_child(forward_button);
	bar->add_child(down_button);

	vec2 size = bar->get_required_size();
	vec2 offset = size + 4;
	bar->set_anchors(ANCHOR_END, ANCHOR_END, ANCHOR_END, ANCHOR_END);
	bar->set_margins(offset.x + 4, offset.y, 4, 4);

	add_child(bar);
	add_child(name_field);

	set_property(p_object);
	set_tab_title("Inspector");
	set_name("Inspector");

	buttons_height = 30.0f;
	bar_area = rect2();
	work_area = rect2();

	history = History<Object*>();
}

PropertyTab::PropertyTab() : PropertyTab(NULL)
{

}


PropertyTab::~PropertyTab()
{
}


vec2 PropertyTab::get_required_size() const
{
	return property_view->get_required_size() + vec2(0.0f, buttons_height);
}

void PropertyTab::set_property(Object* p_object)
{
	history.go_to(p_object);
	property_view->set_property(p_object);

	String title = "";

	Node* n = p_object->cast_to_type<Node*>();
	Resource* r = p_object->cast_to_type<Resource*>();

	if (n)
		title = n->get_name();
	else if (r)
		title = r->get_file();	
	
	name_field->set_text(title);
}

Object* PropertyTab::get_property() const
{
	return property_view->get_property();
}

void PropertyTab::go_back()
{
	set_property(history.go_back());
}

void PropertyTab::go_forward()
{
	set_property(history.go_forward());
}

void PropertyTab::show_history()
{

}

#undef CLASSNAME
#define CLASSNAME PropertyTab

void PropertyTab::bind_methods()
{
	REG_CSTR_OVRLD_1(Object*);

	REG_PROPERTY(property);

	REG_METHOD(go_back);
	REG_METHOD(go_forward);
	REG_METHOD(show_history);

}
