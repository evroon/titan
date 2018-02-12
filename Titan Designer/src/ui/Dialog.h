#pragma once

#include "Control.h"

#include "ImageButton.h"
#include "TextButton.h"
#include "TextField.h"

#include "core\Dictionary.h"

class Container;
class ListView;
class TreeView;
class TreeElement;
class EditableLabel;
class File;
class NumberField;
class Shader;
class PropertyView;

class Dialog : public Control
{
	OBJ_DEFINITION(Dialog, Control);

public:
	Dialog();
	virtual ~Dialog();

	vec2 get_required_size() const;

	void handle_event(UIEvent * p_event);

	void notification(int p_notification) override;

	void set_title(const String& p_title);
	String get_title() const;

	void show();
	void close();

	static void bind_methods();

private:
	rect2 bar;

	ImageButton* close_button;

	float header_offset;

	String title;

	vec2 grab_pos;
	vec2 orig_pos;
	bool is_grabbing;
};

class ConfirmationDialog : public Dialog
{
	OBJ_DEFINITION(ConfirmationDialog, Dialog);

public:
	ConfirmationDialog();

	virtual void chosen_exit();

	static void bind_methods();

private:
	TextButton* ok_button;
	TextButton* cancel_button;
};

class MessageDialog : public Dialog
{
	OBJ_DEFINITION(MessageDialog, Dialog);

public:
	MessageDialog();
	MessageDialog(const String& p_text);

	static void bind_methods();

private:
	Label* desciption_label;

	String desciption;
};

class FileDialog : public ConfirmationDialog
{
	OBJ_DEFINITION(FileDialog, Dialog);

public:
	FileDialog();
	FileDialog(const String& p_file);

	vec2 get_required_size() const override;

	void update_list();

	void list_chosen(int p_index);

	void go_to(const String& p_file);
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

	String file;
};

class ColorPickDialog : public ConfirmationDialog
{
	OBJ_DEFINITION(ColorPickDialog, ConfirmationDialog);

public:
	ColorPickDialog();
	ColorPickDialog(const Color& p_color);

	vec2 get_required_size() const override;

	void chosen_exit() override;

	static void bind_methods();

	void notification(int p_notification) override;

private:

	void set_red(float p_red);
	void set_green(float p_green);
	void set_blue(float p_blue);

	Color color;

	Shader* shader;

	Label* r_label;
	Label* g_label;
	Label* b_label;

	NumberField* r_field;
	NumberField* g_field;
	NumberField* b_field;
};

class TypePickDialog : public ConfirmationDialog
{
	OBJ_DEFINITION(TypePickDialog, Dialog);

public:
	TypePickDialog();

	void notification(int p_notification) override;

	void search(const String& p_search);

	void update_list();

	void tree_chosen(TreeElement* p_element);

	static void bind_methods();

private:
	TreeView* tree;
	TextField* search_field;
};

class SettingsDialog : public ConfirmationDialog
{
	OBJ_DEFINITION(SettingsDialog, ConfirmationDialog);

public:
	SettingsDialog();

	void notification(int p_notification) override;
	void search(const String& p_search);
	void update_list();
	void list_chosen(int p_element);

	static void bind_methods();

private:
	PropertyView* property_view;
	TextField* search_field;
	Dictionary<String, Variant> dictionary;
};

class ProjectSettingsDialog : public SettingsDialog
{
	OBJ_DEFINITION(ProjectSettingsDialog, SettingsDialog);

public:
	ProjectSettingsDialog();

private:

};

class EditorSettingsDialog : public SettingsDialog
{

};