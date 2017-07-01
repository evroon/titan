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

ExplorerTab::ExplorerTab(World2D* p_world)
{
	tree_view = new TreeView;
	buttons = new Container;
	add_button = new ImageButton("EngineCore/New.png");

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

	set_world(p_world);
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

	if (!world)
		return;
	
	tree_view->push_node(world);
}

vec2 ExplorerTab::get_required_size() const
{
	return tree_view->get_required_size();
}

void ExplorerTab::add_button_clicked()
{
	get_canvas()->set_dialog(new TypePickDialog);
}

void ExplorerTab::handle_event(UIEvent* ui_event)
{

}

void ExplorerTab::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:
		
		break;

	case NOTIFICATION_RESIZED:
	case NOTIFICATION_TRANSLATED:

		break;
	}
}

void ExplorerTab::set_world(World2D * p_world)
{
	world = p_world;
}

World2D* ExplorerTab::get_world() const
{
	return world;
}

void ExplorerTab::emit_selected(TreeElement* p_element)
{
	if (p_element)
		emit_signal("selected", world->get_child(p_element->get_text())->cast_to_type<WorldObject*>());
	else
		emit_signal("selected", Variant());
}

void ExplorerTab::select(WorldObject* p_object)
{
	int i = world->get_index(p_object);

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

	REG_SIGNAL("open_file");
	REG_SIGNAL("selected");
}