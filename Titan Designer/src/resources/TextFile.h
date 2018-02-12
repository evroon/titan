#pragma once

#include "Resource.h"

class TextFile : public Resource
{
	OBJ_DEFINITION(TextFile, Resource);

public:
	TextFile(const File& p_file);

	String load(const File& p_file);
	void write(const String& p_source);

	String get_source() const;

private:
	String source;
};