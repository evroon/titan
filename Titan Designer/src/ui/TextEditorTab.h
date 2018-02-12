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
	virtual ~TextEditorTab();

	vec2 get_required_size() const override;

	void handle_event(UIEvent *ui_event) override;

	void notification(int p_notification);

	void show_dialog();

	void open_file(const String &text);

	void savebutton_pressed();

	static void bind_methods();

protected:
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

class ShaderEditorTab : public TextEditorTab
{
	OBJ_DEFINITION(ShaderEditorTab, TextEditorTab);

public:
	ShaderEditorTab();
	ShaderEditorTab(const File& p_file);
	ShaderEditorTab(Shader* p_shader);
	virtual ~ShaderEditorTab();

	void init();

	void compile();

	static void bind_methods();

private:
	Shader* shader;

	ImageButton* compile_button;
};