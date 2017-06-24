#pragma once

#include "core/Reference.h"

#include "File.h"

class Resource : public Referenced
{
	OBJ_DEFINITION(Resource, Referenced);

public:
	Resource() = default;

	virtual void Load() { }
	virtual void Reload() { }
	virtual void Free() { }

	File get_file() const;

	static void bind_methods();

//private:

	File file;
};