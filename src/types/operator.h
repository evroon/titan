#pragma once

#include <string>
#include <functional>

#include "core/variant/variant.h"
#include "types/callable.h"

#define OP_ADD(TYPE_RET, TYPE_L, TYPE_R) \
	static Variant TYPE_L##_##TYPE_R##_##ADD(const Variant &left, const Variant &right) \
	{\
		return left + right;\
	}

#define OP_SUBTRACT(TYPE_RET, TYPE_L, TYPE_R) \
	static Variant TYPE_L##_##TYPE_R##_##SUBTRACT(const Variant &left, const Variant &right) \
	{\
		return left - right;\
	}

#define OP_DIVIDE(TYPE_RET, TYPE_L, TYPE_R) \
	static Variant TYPE_L##_##TYPE_R##_##DIVIDE(const Variant &left, const Variant &right) \
	{\
		return left / right;\
	}

#define OP_MULTIPLY(TYPE_RET, TYPE_L, TYPE_R) \
	static Variant TYPE_L##_##TYPE_R##_##MULTIPLY(const Variant &left, const Variant &right) \
	{\
		return left * right;\
	}

#define OP_LESS(TYPE_L, TYPE_R) \
	static Variant TYPE_L##_##TYPE_R##_##LESS(const Variant &left, const Variant &right) \
	{\
		return left < right;\
	}

#define OP_LEQUAL(TYPE_L, TYPE_R) \
	static Variant TYPE_L##_##TYPE_R##_##LEQUAL(const Variant &left, const Variant &right) \
	{\
		return left <= right;\
	}

#define OP_EQUAL(TYPE_L, TYPE_R) \
	static Variant TYPE_L##_##TYPE_R##_##EQUAL(const Variant &left, const Variant &right) \
	{\
		return left == right;\
	}

#define OP_NOTEQUAL(TYPE_L, TYPE_R) \
	static Variant TYPE_L##_##TYPE_R##_##NOTEQUAL(const Variant &left, const Variant &right) \
	{\
		return left != right;\
	}

#define OP_GREATER(TYPE_L, TYPE_R) \
	static Variant TYPE_L##_##TYPE_R##_##GREATER(const Variant &left, const Variant &right) \
	{\
		return left > right;\
	}

#define OP_GEQUAL(TYPE_L, TYPE_R) \
	static Variant TYPE_L##_##TYPE_R##_##GEQUAL(const Variant &left, const Variant &right) \
	{\
		return left >= right;\
	}

struct OperatorType
{
	enum Type
	{
		UNDEF,

		ADD,
		SUBTRACT,

		MULTIPLY,
		DIVIDE,

		//evaluation
		LESS,
		LEQUAL,
		EQUAL,
		NOTEQUAL,
		GREATER,
		GEQUAL
	};

	OperatorType()
	{
		type = UNDEF;
	}
	OperatorType(const std::string &name)
	{
		if (name == "+")
			type = ADD;
		else if (name == "-")
			type = SUBTRACT;
		else if (name == "*")
			type = MULTIPLY;
		else if (name == "/")
			type = DIVIDE;

		else if (name == "<")
			type = LESS;
		else if (name == ">")
			type = GREATER;
		else if (name == "<=")
			type = GEQUAL;
		else if (name == ">=")
			type = SUBTRACT;
		else if (name == "==")
			type = EQUAL;
		else if (name == "!=")
			type = NOTEQUAL;
	}

	Type type;
	VariantType left_type, right_type;
};

class Operator : public Callable
{
public:
	typedef std::function<Variant(const Variant&, const Variant&)> FUNC;

	Operator(const FUNC &p_func);
	~Operator();

	OperatorType type;

	std::function<Variant(const Variant&, const Variant&)> func;
};
