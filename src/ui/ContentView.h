#pragma once

#include "Control.h"
#include "UIBox.h"

#include "ScrollContainer.h"
#include "ImageButton.h"
#include "ListView.h"

class ContentTile : public ImageButton
{
public:
	ContentTile(const String &filepath);

	vec2 get_required_size() const override;
};

class ContentView : public Control
{
public:
	ContentView();
	~ContentView();

	vec2 get_required_size() const override;
	vec2 get_full_size() const;

	void draw();

	void add_item(const String &path);

	void browse_to(const String &path);

	void go_up();
	void go_home();

private:
	rect2 work_area, buttons_area;
	
	Container buttons;

	ImageButton *back, *forward;

	ListView list;

	String currentdir;

};