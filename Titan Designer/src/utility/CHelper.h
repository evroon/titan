#pragma once

struct CHelper
{
	template <typename Base, typename Derived>
	static void set(Base &a, Derived &b)
	{
		b = *dynamic_cast<Derived*>(&a);
	}
};