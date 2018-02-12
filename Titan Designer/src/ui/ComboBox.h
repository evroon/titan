#pragma once

#include "Control.h"
#include "core/Reference.h"
#include "ContextMenu.h"

class ComboBox : public Control
{
	OBJ_DEFINITION(ComboBox, Control);

public:
	ComboBox();
	virtual ~ComboBox();

	vec2 get_required_size() const override;
	void notification(int p_notification) override;

	void add_item(const String& p_text);

	void set_text(const String& p_text);

	void selected();

	void clear();

	void handle_event(UIEvent *p_ui_event) override;

	static void bind_methods();

private:
	Ref<Texture2D> texture;
	String text;

	ContextMenu* menu;
};

