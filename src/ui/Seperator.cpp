#include "Seperator.h"



Seperator::Seperator()
{
	color = Color::FromRGB(vec3i(50));
}


Seperator::~Seperator()
{
}


vec2 Seperator::get_required_size() const
{
	return vec2(2);
}

void Seperator::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		draw_box(area, color);

		break;
	}

}