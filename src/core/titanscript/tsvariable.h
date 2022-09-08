#pragma once

#include "core/string.h"
#include "utility/stringutils.h"
#include "core/variant/variant.h"


struct TsVariable
{
	TsVariable() = default;
	TsVariable(const StringName &n) { name = n; }
	TsVariable(const StringName &n, const Variant &v) { value = v; name = n; }

	void clean()
	{
		value.clean();
	}
	StringName name;
	Variant value;
};
