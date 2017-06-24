#pragma once

#include "Button.h"

class Toggle : public Button
{
	OBJ_DEFINITION(Toggle, Button);

public:
	Toggle();
	virtual ~Toggle();

	void notification(int p_notification) override;

	vec2 get_required_size() const override;


	static void bind_methods();
};

