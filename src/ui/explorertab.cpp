#include "explorertab.h"

#include "dock.h"
#include "resources/font.h"

#include "imagebutton.h"
#include "textfield.h"

#include "treeview.h"
#include "imagebutton.h"
#include "container.h"
#include "contextmenu.h"

#include "dialog.h"
#include "canvas.h"

#include "world/world.h"

#include "graphics/viewport.h"
#include "graphics/view.h"

ExplorerTab::ExplorerTab(Viewport* p_viewport)
{
	tree_view = new TreeView;
	buttons = new Container;
	add_button = new IconButton("solid/plus");
	search_field = new TextField;

	buttons->add_child(add_button);
	buttons->add_child(search_field);

	//buttons
	vec2 size = buttons->get_required_size();
	vec2 offset = size + 4;
	buttons->set_anchors(ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END, ANCHOR_END);
	buttons->set_margins(4, offset.y, 4, offset.y - size.y);

	//tree_view
	tree_view->set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	tree_view->set_margins(4, 4, 4, offset.y + 4);
	tree_view->connect("selected", this, "emit_selected");
	tree_view->connect("right_clicked", this, "right_clicked");
	tree_view->set_init_expanded_depth(2);

	//add_button
	add_button->set_tip_description("Add Item");
	add_button->connect("clicked", this, "add_button_clicked");

	search_field->set_tip_description("Search for Item");
	search_field->set_empty_text("Search");
	search_field->connect("value_changed", tree_view, "search");

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

Node* ExplorerTab::get_selected_object() const
{
	String name = tree_view->get_selected()->get_text();
	return viewport->get_child(name);
}

void ExplorerTab::delete_element()
{
	Node* item = get_selected_object();

	if (item)
		item->get_parent()->remove_child(item);
}

void ExplorerTab::right_clicked(TreeElement* p_element)
{
	ContextMenu* menu = new ContextMenu;

	Connection del, add;
	del.register_native_method(this, "delete_element");
	Connection duplicate = LAMBDA_CONNECTION_0([this]() {
		get_selected_object()->duplicate();
	});
	add.register_native_method(this, "add_button_clicked");

	menu->add_item("Delete " + p_element->get_text(), del);
	menu->add_item("Duplicate " + p_element->get_text(), duplicate);
	menu->add_item("Add Child", add);

	ACTIVE_CANVAS->set_context_menu(menu, MOUSE->get_position());

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

void ExplorerTab::add_object(const String& p_type)
{
	CSTR_0* c = reinterpret_cast<CSTR_0*>(MMASTER->get_constructor(p_type, 0));
	if (c)
	{
		Variant v = c->operator()();

		if (v.o->derives_from_type<Control*>())
		{
			viewport->get_canvas()->add_control(v);
		}
		else if (v.o->derives_from_type<WorldObject*>())
		{
			viewport->get_world()->add_child(v);
		}
		else {
			get_selected_object()->add_child(v);
		}
		update();
	}
	else
	{
		T_ERROR("constructor not found: " + p_type);
	}
}

void ExplorerTab::emit_selected(TreeElement* p_element)
{
	if (p_element)
		emit_signal("selected", viewport->get_child(p_element->get_text())->cast_to_type<Node*>());
	else
		emit_signal("selected", Variant());
}

void ExplorerTab::select(WorldObject* p_object)
{
	int i = viewport->get_world()->get_index(p_object);

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
	REG_METHOD(right_clicked);
	REG_METHOD(delete_element);

	REG_SIGNAL("open_file");
	REG_SIGNAL("selected");
}