#pragma once

#include "EditableLabel.h"

class TextField;
class TextButton;
class ColorPickDialog;

class ColorField : public PropertyControl
{
	OBJ_DEFINITION(ColorField, PropertyControl);

public:
	ColorField();
	ColorField(Color& p_color);
	ColorField(const Variable& p_variable) : PropertyControl(p_variable) { }

	vec2 get_required_size() const override;

	void notification(int p_notification) override;

	void show_dialog();

	static void bind_methods();

	virtual ~ColorField();

private:
	TextButton* textbutton;
	ColorPickDialog* color_dialog;

	Color* color;
};

class Texture2DField : public PropertyControl
{
	OBJ_DEFINITION(Texture2DField, PropertyControl);

public:
	Texture2DField();
	Texture2DField(Texture2D* p_texture);
	Texture2DField(const Variable& p_variable) : PropertyControl(p_variable) { }

	virtual ~Texture2DField();

	vec2 get_required_size() const override;

	static void bind_methods();

private:
	Ref<Texture2D> texture;

	TextField* textfield;
	TextButton* textbutton;
};

