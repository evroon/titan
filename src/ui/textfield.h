#pragma once

#include "editablelabel.h"

class TextField : public EditableLabel
{
	OBJ_DEFINITION(TextField, EditableLabel);

public:
	TextField();
	TextField(const String &name);
	TextField(const Variable& p_variable);

	virtual ~TextField();

	vec2 get_required_size() const override;

	void notification(int p_notification) override;

	void value_changed() override;

	void handle_input(char c) override;
	void set_text(const String& p_text) override;

	static void bind_methods();
};
