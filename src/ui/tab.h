#pragma once

#include "control.h"

class TabSelector;

class Tab : public Control
{
	OBJ_DEFINITION(Tab, Control);

public:
	Tab();
	virtual ~Tab();

	void set_tab_title(const String& p_title);
	String get_tab_title() const;

	vec2 get_required_size() const override;

private:
	String title;
};
