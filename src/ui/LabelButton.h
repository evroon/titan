#pragma once

#include "UIBox.h"
#include "Button.h"

class LabelButton : public Button
{
	OBJ_DEFINITION(LabelButton, Button);

public:
	LabelButton(const String &text);

	vec2 get_required_size() const override;
	void draw();

	void start_highlight() override;
	void start_selection() override;
	void end_highlight() override;
	void end_selection() override;

	static void bind_methods();

private:
	UIBox box;
	Label label;
};

