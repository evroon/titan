#include "ExplorerTab.h"

#include "Dock.h"
#include "resources/Font.h"

#include "ImageButton.h"
#include "TextField.h"

#include "TreeView.h"
#include "ImageButton.h"
#include "Container.h"

#include "Dialog.h"
#include "Canvas.h"

#include "world/World.h"

#include "graphics\Viewport.h"

ExplorerTab::ExplorerTab(Viewport* p_viewport)
{
	tree_view = new TreeView;
	buttons = new Container;
	add_button = new ImageButton("EngineCore/UI/New.png");

	buttons->add_child(add_button);

	//buttons
	vec2 size = buttons->get_required_size();
	vec2 offset = size + 4;
	buttons->set_anchors(ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END, ANCHOR_END);
	buttons->set_margins(4, offset.y, 4, offset.y - size.y);

	//tree_view
	tree_view->set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	tree_view->set_margins(4, 4, 4, offset.y + 4);
	tree_view->connect("selected", this, "emit_selected");

	//add_button
	add_button->set_tip_description("Add Item");
	add_button->connect("clicked", this, "add_button_clicked");
	
	add_child(tree_view);
	add_child(buttons);

	set_tab_title("Explorer");

	set_viewport(p_viewport);
}

ExplorerTab::ExplorerTab() : ExplorerTab(NULL)
{

}


ExplorerTab::~ExplorerTab()
{
}

void ExplorerTab::add_file(const String& p_path)
{
	
}

void ExplorerTab::update_items()
{
	tree_view->clear();

	if (!viewport)
		return;
	
	tree_view->push_node(viewport);
}

vec2 ExplorerTab::get_required_size() const
{
	return tree_view->get_required_size();
}

void ExplorerTab::add_button_clicked()
{
	TypePickDialog* dialog = new TypePickDialog;
	dialog->connect("type_chosen", this, "add_object");
	get_canvas()->set_dialog(dialog);
}

void ExplorerTab::handle_event(UIEvent* ui_event)
{

}

void ExplorerTab::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_READY:

		update_items();

		break;

	case NOTIFICATION_DRAW:
		
		break;

	case NOTIFICATION_RESIZED:
	case NOTIFICATION_TRANSLATED:

		break;
	}
}

void ExplorerTab::set_viewport(Viewport* p_viewport)
{
	viewport = p_viewport;
}

Viewport* ExplorerTab::get_viewport() const
{
	return viewport;
}

#include "graphics\View.h"

void ExplorerTab::add_object(const String& p_type)
{
	CSTR_0* c = reinterpret_cast<CSTR_0*>(MMASTER->get_constructor(p_type, 0));
	if (c)
	{
		Variant v = reinterpret_cast<CSTR_0*>(MMASTER->get_constructor(p_type, 0))->operator()();

		if (v.o->derives_from_type<Control*>())
		{
			viewport->get_canvas()->add_control(v);
		}
		else if (v.o->derives_from_type<WorldObject*>())
		{
			viewport->get_world()->add_child(v);
		}
	}
	else
	{
		T_ERROR("constructor not found: " + p_type);
	}
}

void ExplorerTab::emit_selected(TreeElement* p_element)
{
	if (p_element)
		emit_signal("selected", viewport->get_child(p_element->get_text())->cast_to_type<WorldObject*>());
	else
		emit_signal("selected", Variant());
}

void ExplorerTab::select(WorldObject* p_object)
{
	int i = viewport->get_index(p_object);

	//if (tree_view->get_selected() == i)
	//	return;

	//tree_view->set_selected(i);
}

#undef CLASSNAME
#define CLASSNAME ExplorerTab

void ExplorerTab::bind_methods()
{
	REG_CSTR(0);
	REG_METHOD(add_button_clicked);
	REG_METHOD(update_items);
	REG_METHOD(select);
	REG_METHOD(emit_selected);
	REG_METHOD(add_object);

	REG_SIGNAL("open_file");
	REG_SIGNAL("selected");
}