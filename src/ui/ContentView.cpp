#include "ContentView.h"

#include "String.h"
#include "utility/StringUtils.h"

ContentTile::ContentTile(const String &filepath) : ImageButton(filepath)
{
	//image.centertype = Image::DEFAULT;
}

vec2 ContentTile::get_required_size() const
{
	return vec2(60);
}

//=========================================================================
//ContentView
//=========================================================================

ContentView::ContentView()
{
	//buttons.set_layout_type(Container::GLUE_HORIZONTAL);
	//buttons.space = 3;
	//buttons.border = 3;

	buttons.add_child(back = new ImageButton("EngineCore/UI/Back.png"));
	buttons.add_child(forward = new ImageButton("EngineCore/UI/Forward.png"));

	go_home();
}

ContentView::~ContentView()
{
}

//void ContentView::init(const rect2 &p_render_area)
//{
//	Control::resize(p_render_area);
//
//	work_area = area.crop(0, 0, 30, 0);
//	buttons_area = area.crop(0, 0, 0, area.size.y * 2 - 30);
//
//	buttons.init(buttons_area);
//	list.init(work_area);
//}
//
//void ContentView::resize(const rect2 &p_render_area)
//{
//	Control::resize(p_render_area);
//	buttons.resize(area);
//	list.resize(area);
//}

vec2 ContentView::get_required_size() const
{
	return vec2(100);
}

vec2 ContentView::get_full_size() const
{
	return vec2(100);
}

void ContentView::draw()
{
	buttons.draw();
	list.draw();
}

//Control* ContentView::raycast(const vec2 &pos) const
//{
//	if (buttons_area.is_in_box(pos))
//		return buttons.raycast(pos);
//	else
//		return list.raycast(pos);
//}

void ContentView::add_item(const String &path)
{
	//ListElement *e = new ListElement;
	//
	////e->container.set_layout_type(Container::GLUE_HORIZONTAL);

	//VariantType type = CONTENT->GetType(path);

	//Image *i;	

	//if (type.is_of_type<TextFile>())
	//	e->container.add_child(i = new Image("EngineCore/TextFile.png"));

	//else if (type.is_of_type<Texture2D>())
	//	e->container.add_child(i = new Image("EngineCore/Picture.png"));
	//
	//else if (type.is_of_type<Shader>())
	//	e->container.add_child(i = new Image("EngineCore/TextFile.png"));

	//else
	//	e->container.add_child(i = new Image("EngineCore/Directory.png"));
	//
	//e->container.add_child(new Label(path));
	//list.add_element(e);
}

void ContentView::browse_to(const String &path)
{
	currentdir = path;

	File file = path;

	Array<File> names = file.listdir();

	for (int c = 0; c < names.size(); c++)
		add_item(names[c]);
}

void ContentView::go_up()
{
	String s = currentdir;
	
	int i = StringUtils::GetLastIndexOf(s, '/');
	s = s.substr(0, i);

	browse_to(s);
}

void ContentView::go_home()
{
	browse_to("assets/");
}