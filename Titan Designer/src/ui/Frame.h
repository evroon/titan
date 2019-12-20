#pragma once

#include "ImageMap.h"

class Frame : public Control
{
public:
	Frame();
	virtual ~Frame();

	void notification(int p_notification) override;

	void build();

	void set_texture(Texture2D *p_texture);
	void set_default_texture();

	void set_color(const Color &p_color);
private:
	Texture2D *texture;
	Color color;
	ImageMap image_map;
};

class Highlighter : public Frame
{
public:
	Highlighter();
};