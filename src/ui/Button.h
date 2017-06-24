#pragma once

#include <string>
#include <functional>

#include "Control.h"

#include "Image.h"
#include "Frame.h"
#include "Label.h"

#include "UICallback.h"

class Button : public Control
{
	OBJ_DEFINITION(Button, Control);

public:
	Button();

	void handle_event(UIEvent *ui_event) override;

	static void bind_methods();

protected:
	bool highlighted = false, selected = false;

	vec2 inner_border = vec2(4);

	virtual void start_highlight();
	virtual void start_selection();
	virtual void end_highlight();
	virtual void end_selection();

private:
};