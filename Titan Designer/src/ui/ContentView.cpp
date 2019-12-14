#include "ContentView.h"

#include "String.h"
#include "utility/StringUtils.h"

ContentTile::ContentTile(const String &filepath) : ImageButton(filepath)
{

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
	buttons.add_child(back = new IconButton("solid/arrow-left"));
	buttons.add_child(forward = new IconButton("solid/arrow-right"));

	go_home();
}

ContentView::~ContentView()
{
}

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

void ContentView::add_item(const String &path)
{

}

void ContentView::browse_to(const String &path)
{
	currentdir = path;

	File file = path;

	Array<File> names = file.listdir();

	for (int c = 0; c < names.size(); c++)
		add_item(names[c]);

	history.push(path);
}

void ContentView::clear_history()
{
	history.clear();
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

void ContentView::go_back()
{
	String s = history.pop();
	future.push(s);
	browse_to(s);
}

void ContentView::go_forward()
{
}
