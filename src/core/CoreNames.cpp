#include "CoreNames.h"

CoreNames* CoreNames::singleton;

CoreNames::CoreNames()
{
}


CoreNames::~CoreNames()
{
}

void CoreNames::init()
{
	singleton = new CoreNames;
}

CoreNames* CoreNames::get_singleton()
{
	return singleton;
}