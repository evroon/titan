#pragma once

#if 0

#include <functional>

#include "methoddefinition.h"

#include "variant.h"

using namespace std;

template <typename T>
class MethodTemplate
{
public:
	typedef function<Variant(T)> m_0;

	template <typename F>
	void bind_method(MethodDefinition def, F f)
	{
		using namespace std::placeholders;

		m_0 binded_method = bind<Variant>(f, _1);

		cout << "YUOP" << endl;
	}

	Vector<m_0> methods;
};

#endif