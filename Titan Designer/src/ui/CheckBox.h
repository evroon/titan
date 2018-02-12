#pragma once

#include "resources/Font.h"
#include "core/ContentManager.h"
#include "PropertyControl.h"
#include "Image.h"


class CheckBox : public PropertyControl
{
	OBJ_DEFINITION(CheckBox, PropertyControl);

public:
	CheckBox();
	CheckBox(bool p_value);
	CheckBox(const Variable& p_variable);
	virtual ~CheckBox() { }

	void init();

	void switch_checked();
	void set_checked(bool p_value);

	vec2 get_required_size() const override;

	void notification(int p_notification) override;

	void handle_event(UIEvent *ui_event) override;

	void draw();

private:
	Texture2D *tex_checked, *tex_unchecked;

	vec2 boxoffset, boxsize, textoffset;
};