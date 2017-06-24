#pragma once

#include "TextBox.h"

#include "ImageButton.h"
#include "Container.h"

#include "Dialog.h"

#include "Tab.h"

class TextEditorTab : public Tab
{
	OBJ_DEFINITION(TextEditorTab, Tab);

public:
	TextEditorTab();
	TextEditorTab(const File& p_file);
	~TextEditorTab();

	vec2 get_required_size() const override;

	void handle_event(UIEvent *ui_event) override;

	void notification(int p_notification);

	void show_dialog();

	void open_file(const String &text);

	void savebutton_pressed();

	static void bind_methods();

private:
	ImageButton* save_button;
	ImageButton* save_all_button;
	ImageButton* open_button;
	ImageButton* new_button;

	TextBox textbox;
	Container buttons;

	rect2 textbox_area;
	TextFile* textfile;

	FileDialog* dialog;
};

