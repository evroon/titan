#pragma once

#include "core/vector.h"
#include "control.h"

class UILayer
{
public:
	UILayer();
	~UILayer();

	Vector<Control> controls;
};
