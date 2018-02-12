#include "ContentTab.h"

#include "Canvas.h"
#include "Container.h"
#include "TileView.h"
#include "TextField.h"
#include "ImageButton.h"

ContentTab::ContentTab() : ContentTab(ASSETS_DIR)
{

}

ContentTab::ContentTab(const File& p_file)
{
	file = p_file;
	home = file;

	directory_icon = CONTENT->LoadTexture(File("EngineCore/UI/Directory.png"));
	file_icon = CONTENT->LoadTexture(File("EngineCore/UI/Picture.png"));

	back_button = new ImageButton("EngineCore/UI/Back.png");
	forward_button = new ImageButton("EngineCore/UI/Forward.png");
	home_button = new ImageButton("EngineCore/UI/Home.png");
	up_button = new ImageButton("EngineCore/UI/Up.png");

	buttons = new Container;
	path_label = new TextField;
	tile_view = new TileView;

	//back_button
	back_button->connect("clicked", this, "go_back");
	back_button->set_tip_description("Back");

	//forward_button
	forward_button->connect("clicked", this, "go_forward");
	forward_button->set_tip_description("Forward");

	//home_button
	home_button->connect("clicked", this, "go_home");
	home_button->set_tip_description("Home");

	//up_button
	up_button->connect("clicked", this, "go_up");
	up_button->set_tip_description("Up");

	//buttons
	buttons->add_child(back_button);
	buttons->add_child(forward_button);
	buttons->add_child(home_button);
	buttons->add_child(up_button);

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
	size = tile_view->get_required_size();
	offset += size + 4;

	tile_view->set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	tile_view->set_margins(4, 4, 4, offset.y - size.y);
	tile_view->connect("chosen", this, "list_chosen");

	add_child(buttons);
	add_child(tile_view);
	add_child(path_label);

	update_list();

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
	tile_view->clear();

	Array<File> files = file.listdir();

	for (int c = 0; c < files.size(); c++)
	{
		File f = files[c];

		VariantType t = CONTENT->GetType(f);
		
		if (f.is_directory())
			tile_view->push_back_item(f.get_name(), directory_icon);
		else if (t.is_of_type<Texture2D>())
			tile_view->push_back_item(f.get_name(), CONTENT->LoadTexture(f));
		else
			tile_view->push_back_item(f.get_name(), file_icon);
	}

	path_label->set_text(file.get_relative_path());
}

void ContentTab::list_chosen(int p_index)
{
	File f = file + tile_view->get_item(p_index)->get_text();

	if (f.is_directory())
		go_to(f);
	else
		emit_signal("file_chosen", f.get_absolute_path());
}

void ContentTab::go_to(const File& p_file)
{
	history.go_to(p_file);
	file = p_file;
	update_list();
}

void ContentTab::go_up()
{
	file.go_up();
	go_to(file);
	update_list();
}

void ContentTab::go_home()
{
	go_to(home);
}

void ContentTab::go_back()
{
	file = history.go_back();
	update_list();
}

void ContentTab::go_forward()
{
	file = history.go_forward();
	update_list();	
}

#undef CLASSNAME
#define CLASSNAME ContentTab

void ContentTab::bind_methods()
{
	REG_CSTR(0);

	REG_SIGNAL("file_chosen");

	REG_METHOD(go_back);
	REG_METHOD(go_forward);
	REG_METHOD(go_up);
	REG_METHOD(go_home);

	REG_METHOD(list_chosen);
}