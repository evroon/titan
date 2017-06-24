#pragma once

#include "Tab.h"

#include "resources/File.h"
#include "ui/TextField.h"

class Container;
class ListView;
class EditableLabel;
class ImageButton;

class ContentTab : public Tab
{
	OBJ_DEFINITION(ContentTab, Tab);

public:
	ContentTab();
	ContentTab(const File & p_file);
	~ContentTab();

	vec2 get_required_size() const;

	void handle_event(UIEvent * p_event);

	void notification(int p_notification) override;

	void update_list();

	void list_chosen(int p_index);

	void go_to(const File& p_file);
	void go_back();
	void go_forward();

	static void bind_methods();

private:
	Container* buttons;
	ListView* list;

	ImageButton* back_button;
	ImageButton* forward_button;

	TextField* path_label;

	Texture2D* directory_icon;
	Texture2D* file_icon;

	File file;
};

