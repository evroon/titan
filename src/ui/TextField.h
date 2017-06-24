#pragma once

#include "EditableLabel.h"

class TextField : public EditableLabel
{
	OBJ_DEFINITION(TextField, EditableLabel);

public:
	TextField();
	TextField(const String &name);
	TextField(const Variable& p_variable);

	virtual ~TextField();

	void notification(int p_notification) override;

	void handle_input(char c) override;
	void set_text(const String& p_text) override;

	static void bind_methods();
};

