#include "MainContainer.h"

#include "TextField.h"
#include "TextButton.h"
#include "CheckBox.h"
#include "TreeView.h"
#include "PropertyTab.h"
#include "GamePreviewTab.h"
#include "ConsoleTab.h"
#include "TextEditorTab.h"
#include "ContentViewTab.h"
#include "ExplorerTab.h"

#include "core/WindowManager.h"

MainContainer::MainContainer()
{
	self_construct();
}

void MainContainer::self_construct()
{
	/*preview_dock = new Dock;
	property_dock = new Dock;
	console_dock = new Dock;
	content_dock = new Dock;
	tool_bar = new ToolBar;

	add_child(preview_dock);
	add_child(property_dock);
	add_child(tool_bar);
	add_child(console_dock);
	add_child(content_dock);

	preview_dock->add_tab(new GamePreviewTab);
	preview_dock->add_tab(new TextEditorTab);
	property_dock->add_tab(new PropertyTab);
	console_dock->add_tab(new ConsoleTab);
	content_dock->add_tab(new ContentViewTab);
	content_dock->add_tab(new ExplorerTab);*/
}

void MainContainer::init(const rect2 &p_render_area)
{
	resize(p_render_area);
}

void MainContainer::resize(const rect2 &p_render_area)
{
	Control::resize(p_render_area);

	rect2 &copy = area;
	work_area = copy.crop(4);
	//init_childs();
}

void MainContainer::init_childs()
{
	int space = 5, tool_bar_height = 40, property_width = 400, console_height = 200, property_height = 600;

	preview_dock->init(rect2(
		area.get_left() + space,
		area.get_right() - space - property_width,
		area.get_top() - tool_bar_height - space,
		area.get_bottom() + console_height + space));

	property_dock->init(rect2(
		area.get_right() - property_width, 
		area.get_right() - space,
		area.get_top() - tool_bar_height - space,
		area.get_top() - tool_bar_height - space - property_height));

	content_dock->init(rect2(
		area.get_right() - property_width,
		area.get_right() - space,
		area.get_top() - tool_bar_height - space * 2.0f - property_height,
		area.get_bottom() + console_height + space));

	console_dock->init(rect2(
		area.get_left() + space, 
		area.get_right() - space,
		area.get_bottom() + console_height,
		area.get_bottom() + space));

	tool_bar->init(rect2(
		area.get_left(), 
		area.get_right(),
		area.get_top(), 
		area.get_top() - tool_bar_height));
}

void MainContainer::open_file(const string &filename)
{
	VariantType type = CONTENT->GetType(filename);

	if (type.get_type_name().get_source() == "Shader" || type.get_type_name().get_source() == "TextFile")
	{
		TextEditorTab *edit_tab = new TextEditorTab;

		preview_dock->add_tab(edit_tab);
		preview_dock->post_init_tab(edit_tab);

		edit_tab->open_file(filename);
	}
}