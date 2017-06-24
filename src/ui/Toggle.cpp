#include "Toggle.h"



Toggle::Toggle()
{
}


Toggle::~Toggle()
{
}

void Toggle::notification(int p_notification)
{
}

vec2 Toggle::get_required_size() const
{
	return vec2();
}

#undef CLASSNAME
#define CLASSNAME Toggle

void Toggle::bind_methods()
{
	REG_CSTR(0);
}
