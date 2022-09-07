#pragma once

#include "tab.h"

#include "resources/file.h"
#include "ui/textfield.h"

#include "utility/history.h"

class Container;
class TileView;
class EditableLabel;
class ImageButton;

class ContentTab : public Tab
{
	OBJ_DEFINITION(ContentTab, Tab);

public:
	ContentTab();
	ContentTab(const File& p_file);
	~ContentTab();

	vec2 get_required_size() const override;

	void handle_event(UIEvent* p_event) override;

	void notification(int p_notification) override;

	void update_list();

	void list_chosen(int p_index);

	void go_to(const File& p_file);
	void go_up();
	void go_home();
	void go_back();
	void go_forward();

	static void bind_methods();

private:
	Container* buttons;
	TileView* tile_view;

	ImageButton* back_button;
	ImageButton* forward_button;
	ImageButton* up_button;
	ImageButton* home_button;

	TextField* path_label;

	Texture2D* directory_icon;
	Texture2D* file_icon;

	File file;
	File home;

	History<String> history;
};
