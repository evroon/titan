#pragma once

#include "String.h"
#include "utility/StringUtils.h"
#include "core/variant/Variant.h"


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