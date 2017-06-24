#include "MethodBinder.h"

#include "resources/Font.h"


MethodBinder::MethodBinder()
{
}


MethodBinder::~MethodBinder()
{
}

void MethodBinder::bind_all()
{
	Font::bind_methods();
}