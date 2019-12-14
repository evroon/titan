#pragma once

#include "EditableLabel.h"
#include "Dialog.h"

class TextField;
class TextButton;
class ColorPickDialog;

class ColorField : public PropertyControl
{
	OBJ_DEFINITION(ColorField, PropertyControl);

public:
	ColorField();
	ColorField(Color& p_color);
	ColorField(const Variable& p_variable);

	void init();
	void color_chosen(const Color& p_color);

	vec2 get_required_size() const override;

	void notification(int p_notification) override;

	void show_dialog();

	static void bind_methods();

	virtual ~ColorField();

private:
	TextButton* textbutton;
	ColorPickDialog* color_dialog;
};

class Texture2DField : public PropertyControl
{
	OBJ_DEFINITION(Texture2DField, PropertyControl);

public:
	Texture2DField();
	Texture2DField(Texture2D* p_texture);
	Texture2DField(const Variable& p_variable);

	virtual ~Texture2DField();

	void init();

	vec2 get_required_size() const override;

	void notification(int p_notification);

	void load_texture();

	void load_button_clicked();

	void file_chosen(const String& p_path);

	static void bind_methods();

private:
	Ref<Texture2D> texture;

	TextButton* load_button;
	TextButton* open_button;

	FileDialog* file_dialog;
};

class ObjectField : public PropertyControl
{
	OBJ_DEFINITION(ObjectField, PropertyControl);

public:
	ObjectField();
	ObjectField(Object* p_object);
	ObjectField(const Variable& p_variable);

	virtual ~ObjectField();

	void init();

	void open_button_clicked();

	void load_button_clicked();

	void file_chosen(const String & p_path);

	vec2 get_required_size() const override;

	static void bind_methods();

private:
	Ref<Object> object;

	TextField* textfield;

	TextButton* load_button;
	ImageButton* open_button;

	FileDialog* file_dialog;
};

class ShaderField : public PropertyControl
{
	OBJ_DEFINITION(ShaderField, PropertyControl);

public:
	ShaderField();
	ShaderField(Shader* p_shader);
	ShaderField(const Variable& p_variable);

	virtual ~ShaderField();

	void init();

	void open_button_clicked();

	vec2 get_required_size() const override;

	static void bind_methods();

private:
	Ref<Shader> object;

	TextField* textfield;
	TextButton* open_button;

	FileDialog* file_dialog;
};
