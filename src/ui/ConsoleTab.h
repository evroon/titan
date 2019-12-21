#pragma once

#include "TextBox.h"
#include "Tab.h"

class ConsoleTab : public Tab
{
	OBJ_DEFINITION(ConsoleTab, Tab);

public:
	ConsoleTab();
	~ConsoleTab();

	vec2 get_required_size() const override;

	void handle_event(UIEvent *ui_event) override;

	void log(int p_index);

	static void bind_methods();

private:
	TextBox *textbox;
};

