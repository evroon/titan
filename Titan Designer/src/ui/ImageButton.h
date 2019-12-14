#pragma once

#include "FramedButton.h"

class ImageButton : public FramedButton
{
	OBJ_DEFINITION(ImageButton, FramedButton);

public:
	ImageButton(const String &tex_name);
	ImageButton(Texture2D* p_texture);

	void notification(int p_notification) override;

	void set_image(Texture2D* p_texture);
	Texture2D* get_image() const;

	vec2 get_required_size() const override;

protected:
	Texture2D* texture;

private:

	float texture_margins[4];

};