#include "variant.h"

#include "core/tmessage.h"
#include "core/corenames.h"

//Array methods
Variant Variant::GetElement(const Variant &index)
{
	if (index.type == Variant::INT)
	{
		if (index.i >= static_cast<signed int>(a->size()) || i < 0)
		{
			T_ERROR("Array index out of bounds");
			return NULL_VAR;
		}
		if (type == Variant::ARRAY)
			return a->operator[](index.i);

		return *this;
	}

	return NULL_VAR;
}

void Variant::clear()
{
	//if (type != ARRAY)
	//	T_ERROR("Cannot call method 'clear' on a non-array variant");
	//else
		a->clear();
}

void Variant::push_back(const Variant &var)
{
	if (type != ARRAY)
	{
		//Transform this into an array
		type = ARRAY;
		a = new Array<Variant>;
		//a.push_back(*this);
	}

	a->push_back(var);
}

int Variant::size()
{
	if (type == ARRAY)
		return a->size();
	else
		return 1;
}
