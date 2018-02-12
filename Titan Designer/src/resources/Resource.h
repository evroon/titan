#pragma once

#include "core/Reference.h"

#include "File.h"

class Resource : public Referenced
{
	OBJ_DEFINITION(Resource, Referenced);

public:
	Resource() = default;

	virtual void load() { }
	virtual void reload() { }
	virtual void free() { }

	void set_file(const String& p_file);
	String get_file() const;

	static void bind_methods();

protected:

	File file;
};