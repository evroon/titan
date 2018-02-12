#include "Real.h"

#include "String.h"

Real::Real(const String &text)
{
	if (!StringUtils::Contains(text, '.'))
	{
		i = atoi(text.c_str());
		t = INT;
	}
	else
	{
		f = StringUtils::StringToFloat(text);
		t = FLOAT;
	}
}

String Real::to_string() const
{
	return t == INT ? StringUtils::IntToString(i) : StringUtils::FloatToString(f);
}


Real::operator String() const
{
	return to_string();
}