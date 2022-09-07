#include "contentviewtab.h"

#include "dock.h"

ContentViewTab::ContentViewTab()
{
	content_view = new ContentView;
}


ContentViewTab::~ContentViewTab()
{
}

vec2 ContentViewTab::get_required_size() const
{
	return vec2(100);
}