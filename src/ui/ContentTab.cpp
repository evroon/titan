#include "ContentTab.h"

#include "Canvas.h"
#include "Container.h"
#include "ListView.h"
#include "TextField.h"
#include "ImageButton.h"

ContentTab::ContentTab() : ContentTab("")
{

}

ContentTab::ContentTab(const File& p_file)
{
	file = ASSETS_DIR;

	directory_icon = CONTENT->LoadTexture(File("EngineCore/UI/Directory.png"));
	file_icon = CONTENT->LoadTexture(File("EngineCore/UI/Picture.png"));

	back_button = new ImageButton("EngineCore/UI/Back.png");
	forward_button = new ImageButton("EngineCore/UI/Forward.png");
	buttons = new Container;
	path_label = new TextField;
	list = new ListView;

	//back_button
	back_button->connect("clicked", this, "go_back");
	back_button->set_tip_description("Back");

	//forward_button
	forward_button->connect("clicked", this, "go_forward");
	back_button->set_tip_description("Forward");

	//buttons
	buttons->add_child(back_button);
	buttons->add_child(forward_button);

	vec2 size = buttons->get_required_size();
	vec2 offset = size + 4;
	buttons->set_anchors(ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END, ANCHOR_END);
	buttons->set_margins(4, offset.y, 4, offset.y - size.y);

	//path_label
	size = path_label->get_required_size();
	offset += size + 4;

	path_label->set_anchors(ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END, ANCHOR_END);
	path_label->set_margins(4, offset.y, 4, offset.y - size.y);

	//list
	size = list->get_required_size();
	offset += size + 4;

	list->set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	list->set_margins(4, 4, 4, offset.y - size.y);
	list->connect("chosen", this, "list_chosen");

	add_child(buttons);
	add_child(list);
	add_child(path_label);

	go_to(p_file);

	set_tab_title("Content");
}


ContentTab::~ContentTab()
{
}

vec2 ContentTab::get_required_size() const
{
	return vec2();
}

void ContentTab::handle_event(UIEvent* p_event)
{
	
}

void ContentTab::notification(int p_notification)
{
}

void ContentTab::update_list()
{
	list->clear();

	Array<File> files = file.listdir();

	for (int c = 0; c < files.size(); c++)
	{
		File f = files[c];
		list->push_back_item(f.get_name(), f.is_directory() ? directory_icon : file_icon);
	}

	path_label->set_text(file.get_relative_path());
}

void ContentTab::list_chosen(int p_index)
{
	File f = file + list->get_item(p_index)->get_text();

	if (f.is_directory())
		go_to(f);
	else
		emit_signal("file_chosen", f.get_absolute_path());
}

void ContentTab::go_to(const File& p_file)
{
	file = p_file;
	update_list();
}

void ContentTab::go_back()
{
	file.go_up();
	update_list();
}

void ContentTab::go_forward()
{
	//file.go_up();
}

#undef CLASSNAME
#define CLASSNAME ContentTab

void ContentTab::bind_methods()
{
	REG_CSTR(0);

	REG_SIGNAL("file_chosen");

	REG_METHOD(go_back);
	REG_METHOD(go_forward);
	REG_METHOD(list_chosen);
}
