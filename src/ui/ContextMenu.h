#pragma once

#include "LabelButton.h"
#include "Control.h"
#include "Container.h"
#include "UIBox.h"

class ContextMenu : public Control
{
	OBJ_DEFINITION(ContextMenu, Control);

public:
	struct ContextMenuItem
	{
		bool seperator;
		String text;
		rect2 area;
		Texture2D* icon;
	};

	ContextMenu();
	virtual ~ContextMenu();

	vec2 get_required_size() const override;
	void handle_event(UIEvent *p_event) override;

	void position_items();

	int get_item(const vec2& p_pos) const;

	void notification(int p_notification) override;

	void add_item(const String& p_text);
	void add_item(Texture2D* p_icon, const String& p_text);
	void add_seperator();

	bool is_empty();

	void close();

	static void bind_methods();

private:
	int selected;
	int highlighted;

	float left_margin;
	float right_margin;

	float seperator_height;
	float option_height;

	Array<ContextMenuItem> items;

	Color background_color;
	Color selection_color;
	Color highlight_color;
	Color border_color;
	Color seperator_color;

	Font* font;
};

class ContextTip : public Control
{
	OBJ_DEFINITION(ContextTip, Control);

public:
	ContextTip();
	ContextTip(const String& p_description);
	virtual ~ContextTip();

	vec2 get_required_size() const override;
	void handle_event(UIEvent *p_event) override;

	void notification(int p_notification) override;

	void close();

	static void bind_methods();

private:
	String description;

	float left_margin;
	float right_margin;
	float top_margin;
	float bottom_margin;

	Color background_color;
	Color border_color;
};