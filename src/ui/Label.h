#pragma once

#include "resources/Font.h"
#include "Control.h"
#include "core/ContentManager.h"
#include "Image.h"

class Label : public Control
{
	OBJ_DEFINITION(Label, Control)

public:
	Label();
	Label(const String &t);
	Label(const String &t, Font *f);
	
	void handle_event(UIEvent * ui_event) override;

	void set_font(Font* p_font);
	Font* get_font() const;

	void set_color(const Color& p_color);
	Color get_color() const;

	int get_index(float x) const;
	float get_position_x(int index) const;

	void set_centering_type(const Image::CenteringType & centering_type);

	void set_text(const String & p_text);
	String get_text() const;

	vec2 get_required_size() const override;

	bool is_in_text(const vec2 & pos);

	void notification(int p_notification) override;

	static void bind_methods();

private:
	Font *font;
	Color color;
	String text;
};