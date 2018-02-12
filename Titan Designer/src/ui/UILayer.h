#pragma once

#include "core/Vector.h"
#include "Control.h"

class UILayer
{
public:
	UILayer();
	~UILayer();

	Vector<Control> controls;
};

