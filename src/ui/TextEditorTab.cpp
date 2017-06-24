#include "TextEditorTab.h"

#include "Dock.h"
#include "Seperator.h"

TextEditorTab::TextEditorTab() : TextEditorTab("")
{

}

TextEditorTab::TextEditorTab(const File& p_file)
{
	save_button = new ImageButton("EngineCore/Save.png");
	save_button->connect("clicked", this, "savebutton_pressed");

	save_all_button = new ImageButton("EngineCore/SaveAll.png");
	save_all_button->connect("clicked", this, "savebutton_pressed");

	new_button = new ImageButton("EngineCore/New.png");
	new_button->connect("clicked", this, "open_file");

	open_button = new ImageButton("EngineCore/Open.png");
	open_button->connect("clicked", this, "show_dialog");

	add_child(&textbox);
	add_child(&buttons);

	textbox.set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	textbox.set_margins(4, 4, 4, 42);
	textbox.set_line_numbers_enabled(true);

	buttons.set_glue_vert(false);
	buttons.set_anchors(ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END, ANCHOR_END);
	buttons.set_margins(4, 4 + 34, 4, 4);

	buttons.add_child(new_button);
	buttons.add_child(open_button);
	buttons.add_child(new Seperator);
	buttons.add_child(save_button);
	buttons.add_child(new ImageButton("EngineCore/SaveAll.png"));

	open_file(p_file);
}

TextEditorTab::~TextEditorTab()
{
}

vec2 TextEditorTab::get_required_size() const
{
	return vec2(200);
}

void TextEditorTab::handle_event(UIEvent *ui_event)
{

}

void TextEditorTab::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		break;

	case NOTIFICATION_TRANSLATED:
	case NOTIFICATION_RESIZED:

		break;
	}
}

void TextEditorTab::show_dialog()
{
	dialog = new FileDialog;
	dialog->connect("file_chosen", this, "open_file");

	dialog->show();
}

void TextEditorTab::open_file(const String &text)
{
	textfile = CONTENT->LoadTextFile(text);

	textbox.set_text(textfile->get_source());

	set_tab_title(textfile->get_file().get_relative_path());
}

void TextEditorTab::savebutton_pressed()
{
	if (textfile)
		textfile->write(textbox.get_text());
}

#undef CLASSNAME
#define CLASSNAME TextEditorTab

void TextEditorTab::bind_methods()
{
	REG_CSTR(0);

	REG_METHOD(savebutton_pressed);
	REG_METHOD(open_file);
	REG_METHOD(show_dialog);
	
}
