#pragma once

#include "Tab.h"

class Container;
class TreeView;
class ImageButton;
class World;
class TreeElement;

class ExplorerTab : public Tab
{
	OBJ_DEFINITION(ExplorerTab, Tab);

public:
	ExplorerTab();
	ExplorerTab(World * p_world);

	virtual ~ExplorerTab();

	void add_file(const String & p_path);

	void update_items();

	vec2 get_required_size() const override;

	void add_button_clicked();

	void handle_event(UIEvent *ui_event) override;

	void notification(int p_notification);

	void set_world(World* p_world);
	World* get_world() const;

	void emit_selected(TreeElement* p_element);

	void select(WorldObject * p_object);

	static void bind_methods();

private:
	TreeView* tree_view;
	Container* buttons;

	World* world;

	ImageButton* add_button;
};

