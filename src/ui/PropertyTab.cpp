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
	add_child(name_field);

	name_field->set_anchors(ANCHOR_BEGIN, ANCHOR_END, ANCHOR_BEGIN, ANCHOR_END);
	name_field->set_margins(4, 36, 196, 4);

	set_property(p_object);

	set_tab_title("Inspector");
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
	property_view->set_property(p_object);

	String title = "";

	Node* n = p_object->cast_to_type<Node*>();
	Resource* r = p_object->cast_to_type<Resource*>();

	if (n)
		title = n->get_name();
	else if (r)
		title = r->get_file().get_name();	
	
	name_field->set_text(title);
}

Object* PropertyTab::get_property() const
{
	return property_view->get_property();
}

#undef CLASSNAME
#define CLASSNAME PropertyTab

void PropertyTab::bind_methods()
{
	REG_CSTR_OVRLD_1(Object*);
	REG_PROPERTY(property);
}
