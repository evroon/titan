#include "Resource.h"

#undef CLASSNAME
#define CLASSNAME Resource

File Resource::get_file() const
{
	return file;
}

void Resource::bind_methods()
{
	//REG_METHOD(get_file);
}
