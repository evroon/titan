#pragma once

#include "Button.h"

class FramedButton : public Button
{
	OBJ_DEFINITION(FramedButton, Button);

public:
	FramedButton();
	~FramedButton();

	void notification(int p_notification) override;
	
	void set_color(const Color & p_color);

	void start_highlight() override;
	void start_selection() override;
	void end_highlight() override;
	void end_selection() override;

protected:
	Color color;
};

