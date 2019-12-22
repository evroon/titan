#include "Variant.h"

#include "core/TMessage.h"
#include "core/CoreNames.h"
#include "core/variant/VariantType.h"

//Operators
//Initialize
void Variant::operator=(const Variant &r)
{
	reference(r);
}

//Comparison Evaluation
bool Variant::eval_comp(const int EvalType, const Variant &right) const
{
	if (right.type != type)
	{
		if (EvalType == EQUAL)
			return false;
		else if (EvalType == NOTEQUAL)
			return true;
	}

	switch (type)
	{
	case INT:
		switch (EvalType)
		{
			case LESS: return i < right.i;
			case LEQUAL: return i <= right.i;
			case EQUAL: return i == right.i;
			case NOTEQUAL: return i != right.i;
			case GREATER: return i > right.i;
			case GEQUAL: return i >= right.i;
		}
		break;
	case FLOAT:
		switch (EvalType)
		{
		case LESS: return f < right.f;
		case LEQUAL: return f <= right.f;
		case EQUAL: return f == right.f;
		case NOTEQUAL: return f != right.f;
		case GREATER: return f > right.f;
		case GEQUAL: return f >= right.f;
		}
		break;
	case BOOL:
		switch (EvalType)
		{
			case EQUAL: return b == right.b;
			case NOTEQUAL: return b != right.b;
		}
		break;
	case STRING:
		switch (EvalType)
		{
			case EQUAL: return *s == *right.s;
			case NOTEQUAL: return *s != *right.s;
		}
		break;
	case VEC2:
		switch (EvalType)
		{
			case EQUAL: return *v2 == *right.v2;
			case NOTEQUAL: return *v2 != *right.v2;
		}
		break;
	case VEC3:
		switch (EvalType)
		{
			case EQUAL: return *v3 == *right.v3;
			case NOTEQUAL: return *v3 != *right.v3;
		}
		break;
	case VEC4:
		switch (EvalType)
		{
			case EQUAL: return *v4 == *right.v4;
			case NOTEQUAL: return *v4 != *right.v4;
		}
		break;
	case OBJECT:
		switch (EvalType)
		{
		case EQUAL: return o == right.o;
		case NOTEQUAL: return o != right.o;
		}
		break;
	}

	T_ERROR("Comparison not defined");
	return false;
}

bool Variant::operator==(const Variant &r) const
{
	return eval_comp(EQUAL, r);
}
bool Variant::operator!=(const Variant &r) const
{
	return eval_comp(NOTEQUAL, r);
}
bool Variant::operator>(const Variant &r) const
{
	return eval_comp(GREATER, r);
}
bool Variant::operator>=(const Variant &r) const
{
	return eval_comp(GEQUAL, r);
}
bool Variant::operator<(const Variant &r) const
{
	return eval_comp(LESS, r);
}
bool Variant::operator<=(const Variant &r) const
{
	return eval_comp(LEQUAL, r);
}

//Operation Evaluation
Variant Variant::operate(const int OPType, const Variant &right) const
{
	Real n_l, n_r;

	switch (type)
	{
		case BOOL:
			break;
		case INT:
			switch (right.type)
			{
			case INT:

				switch (OPType)
				{
				case ADD:
					return i + right.i;
				case SUBTRACT:
					return i - right.i;
				case MULTIPLY:
					return i * right.i;
				case DIVIDE:
					return i / right.i;
				}
				break;
				
			case VEC2:
				switch (OPType)
				{
				case MULTIPLY:
					return *right.v2 * to_float(i);
				case DIVIDE:
					return *right.v2 * to_float(i);
				}
				break;
			}
			break;
		case FLOAT:

			switch (right.type)
			{
			case FLOAT:
				switch (OPType)
				{
				case ADD:
					return f + right.f;
				case SUBTRACT:
					return f - right.f;
				case MULTIPLY:
					return f * right.f;
				case DIVIDE:
					return f / right.f;
				}
				break;
			case VEC2:

				switch (OPType)
				{
				case ADD:
					return *right.v2 + f;
				case SUBTRACT:
					return *right.v2 - f;
				case MULTIPLY:
					return *right.v2 * f;
				case DIVIDE:
					return *right.v2 / f;
				}
				break;
			}
			break;
		case STRING:
			if (OPType == ADD)
				return *s + *right.s;
			else if (OPType == MULTIPLY)
				return *s * right.i;
			break;
		case VEC2:
			if (right.type == VEC2)
			{
				switch (OPType)
				{
					case ADD: 
						return new vec2(*v2 + *right.v2);
					case SUBTRACT: 
						return new vec2(*v2 - *right.v2);
					case MULTIPLY: 
						return new vec2(*v2 * *right.v2);
					case DIVIDE: 
						return new vec2(*v2 / *right.v2);
				}
			}
			else if (right.type == FLOAT)
			{
				switch (OPType)
				{
					case ADD: 
						return new vec2(*v2 + right.f);
					case SUBTRACT: 
						return new vec2(*v2 - right.f);
					case MULTIPLY: 
						return new vec2(*v2 * right.f);
					case DIVIDE: 
						return new vec2(*v2 / right.f);
				}
			}
			break;
		case VEC3:
			if (right.type == VEC3)
			{
				switch (OPType)
				{
					case ADD: 
						return new vec3(*v3 + *right.v3);
					case SUBTRACT: 
						return new vec3(*v3 - *right.v3);
					case MULTIPLY: 
						return new vec3(*v3 * *right.v3);
					case DIVIDE:
						return new vec3(*v3 / *right.v3);
				}
			}
			else if (right.type == FLOAT)
			{
				switch (OPType)
				{
					case ADD: 
						return new vec3(*v3 + right.f);
					case SUBTRACT: 
						return new vec3(*v3 - right.f);
					case MULTIPLY: 
						return new vec3(*v3 * right.f);
					case DIVIDE: 
						return new vec3(*v3 / right.f);
				}
			}		
			break;
		case VEC4:
			if (right.type == VEC4)
			{
				switch (OPType)
				{
					case ADD: 
						return new vec4(*v4 + *right.v4);
					case SUBTRACT: 
						return new vec4(*v4 - *right.v4);
					case MULTIPLY: 
						return new vec4(*v4 * *right.v4);
					case DIVIDE: 
						return new vec4(*v4 / *right.v4);
				}
			}
			else if (right.type == FLOAT)
			{
				switch (OPType)
				{
					case ADD: 
						return new vec4(*v4 + right.f);
					case SUBTRACT: 
						return new vec4(*v4 - right.f);
					case MULTIPLY: 
						return new vec4(*v4 * right.f);
					case DIVIDE: 
						return new vec4(*v4 / right.f);
				}
			}
			break;
		case MAT4:
			if (right.type == MAT4)
			{
				switch (OPType)
				{
					case ADD: 
						return new mat4(*m4 + *right.m4);
					case SUBTRACT: 
						return new mat4(*m4 - *right.m4);
					case MULTIPLY: 
						return new mat4(*m4 * *right.m4);
				}
				break;
			}
	}
	return 0;
}
Variant Variant::operator+(const Variant &r) const
{
	return operate(ADD, r);
}
Variant Variant::operator-(const Variant &r) const
{
	return operate(SUBTRACT, r);
}
Variant Variant::operator*(const Variant &r) const
{
	return operate(MULTIPLY, r);
}
Variant Variant::operator/(const Variant &r) const
{
	return operate(DIVIDE, r);
}

Variant Variant::operator[](int index)
{
	return GetElement(index);
}

Variant Variant::operator[](const Variant &r)
{
	return GetElement(r);
}

Variant Variant::operator!() const
{
	return !operator bool();
}

String Variant::ToString() const
{
	String result = "";

	switch (type)
	{
	case INT:
		result = i;
		break;

	case FLOAT:
		result = StringUtils::FloatToString(f);
		break;

	case BOOL:
		result = b ? "true" : "false";
		break;

	case STRING:
		result = *s;
		break;

	case VEC2:
		result = v2->to_string();
		break;

	case VEC3:
		result = v3->to_string();
		break;

	case VEC4:
		result = v4->to_string();
		break;

	case MAT4:
		result = m4->ToString();
		break;

	case COLOR:
		result = c->toString();
		break;

	case TRANSFORM:
		result = t->ToString();
		break;

	case OBJECT:
		break;

	case ARRAY:
		if (a->size() == 0)
		{
			result = "{ }";
			break;
		}
		else if (a->size() == 1)
		{
			result = a->operator[] (0).ToString();
			break;
		}

		result = "{ ";

		for (int c = 0; c < a->size() - 1; c++)
			result += a->operator[](c).ToString() + ", ";

		result += a->operator[](a->size() - 1).ToString() + " }";
		break;

	default:
		break;
	}
	return result;
}

void Variant::convert_error(const String& p_type) const
{
	T_ERROR(String("Invalid Conversion, could not convert ") + operator String() + String(" to ") + p_type);
}

//Conversions
Variant::operator bool() const
{
	switch (type)
	{
		case INT:
			return i != 0;
		case BOOL:
			return b;
		default:
			return isdef();
	}
	convert_error("bool");
	return 0;
}
Variant::operator int() const
{
	switch (type)
	{
		case INT:
			return i;
		case BOOL:
			return b;
		//case STRING: return (int) s;
	}
	convert_error("int");
	return 0;
}
Variant::operator float() const
{
	switch (type)
	{
		case INT:
			return float(i);
		case FLOAT:
			return f;
		case BOOL:
			return b ? 1.0f : 0.0f;
		//case STRING: return (double)s;
	}
	convert_error("float");
	return 0;
}
Variant::operator double() const
{
	switch (type)
	{
	case FLOAT:
		return double(f);
	case BOOL:
		return b ? 1.0f : 0.0f;
		//case STRING: return (double)s;
	}
	convert_error("double");
	return 0;
}
Variant::operator Real() const
{
	switch (type)
	{
		case INT:
			return i;
		case FLOAT:
			return f;
	}
	convert_error("Real");
	return 0;
}
Variant::operator String&() const
{
	return *s;
}
Variant::operator vec2()
{
	return operator vec2 &();
}
Variant::operator vec3()
{
	return operator vec3 &();	
}
Variant::operator vec4()
{
	return operator vec4 &();
}
Variant::operator mat4()
{
	return operator mat4 &();
}
Variant::operator Color()
{
	return operator Color &();
}
Variant::operator Transform()
{
	return operator Transform &();
}
Variant::operator vec2&() const
{
	if (type == VEC2)
		return *v2;

	convert_error("vec2");
	return *new vec2();
}
Variant::operator vec3&() const
{
	if (type == VEC3)
		return *v3;

	convert_error("vec3");
	return *new vec3();
}
Variant::operator vec4&() const
{
	if (type == VEC4)
		return *v4;

	convert_error("vec4");
	return *new vec4();
}
Variant::operator mat4&() const
{
	if (type == MAT4)
		return *m4;

	convert_error("mat4");
	return *new mat4();
}
Variant::operator Color&() const
{
	if (type == COLOR)
		return *c;

	convert_error("Color");
	return *new Color;
}
Variant::operator Transform&() const
{
	if (type == TRANSFORM)
		return *t;

	convert_error("Transform");
	return *new Transform;
}
Variant::operator Object*() const
{
	if (type == OBJECT)
		return o;
	else if (type == UNDEF)
		return nullptr;
	else if (type == INT && i == 0)
		return nullptr;
	

	convert_error("Object");
	return nullptr;
}