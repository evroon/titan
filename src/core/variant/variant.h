#pragma once

#include <string>

#include "core/string.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"
#include "math/transform.h"
#include "math/real.h"
#include "math/color.h"

#include "core/array.h"

class Object;
class VariantType;
struct VariantPtr;
struct VariantPtrExt;

#define NULL_VAR Variant()

class Variant
{
public:
	Variant();
	Variant(const Variant& r);

	//Value
	Variant(const VariantPtrExt &p);
	Variant(bool p_b);
	Variant(unsigned p_i);
	Variant(int p_i);
	Variant(float p_d);
	Variant(const Real &p_r);
	Variant(const String &p_s);
	Variant(const vec2 &p_v2);
	Variant(const vec3 &p_v3);
	Variant(const vec4 &p_v4);
	Variant(const mat4 &p_m4);
	Variant(const Color &p_c);
	Variant(const Transform &p_t);
	Variant(const Array<Variant> &p_a);

	//Reference
	Variant(Object *p_g);
	Variant(Real *p_r);
	Variant(String *p_s);
	Variant(vec2 *p_v2);
	Variant(vec3 *p_v3);
	Variant(vec4 *p_v4);
	Variant(mat4 *p_m4);
	Variant(Color *p_c);
	Variant(Transform *p_t);
	Variant(Array<Variant> *p_a);

	~Variant() { }

	enum Type
	{
		UNDEF = 0,
		BOOL = 1,
		INT = 2,
		FLOAT = 3,
		STRING = 4,
		VEC2 = 5,
		VEC3 = 6,
		VEC4 = 7,
		MAT4 = 8,
		COLOR = 9,
		TRANSFORM = 10,
		OBJECT = 11,
		ARRAY = 12
	};

	enum EvaluationType
	{
		LESS,
		LEQUAL,
		EQUAL,
		NOTEQUAL,
		GREATER,
		GEQUAL,

		AND,
		OR
	};

	enum OperatorType
	{
		ADD,
		SUBTRACT,

		MULTIPLY,
		DIVIDE
	};

	union
	{
		void *ptr;
		bool b;
		int i;
		float f;
		String *s;
		vec2 *v2;
		vec3 *v3;
		vec4 *v4;
		mat4 *m4;
		Color *c;
		Transform *t;
		Object *o;

		Array<Variant> *a;
	};

	//Memory Management
	void reference(const Variant &ref);
	void copy(const Variant &ref);
	Variant reference();
	Variant copy();
	void clean();

	void free();

	//Operators
	Variant operate(const int OPType, const Variant &right) const;

	Variant operator!() const;

	Variant operator[](int index);
	Variant operator[](const Variant &r);

	//Comparison
	bool eval_comp(const int EvalType, const Variant &right) const;

	//temp

	Variant operator+(const Variant &right) const;
	Variant operator-(const Variant &right) const;
	Variant operator*(const Variant &right) const;
	Variant operator/(const Variant &right) const;
	bool operator==(const Variant &right) const;
	bool operator!=(const Variant &right) const;
	bool operator>(const Variant &right) const;
	bool operator>=(const Variant &right) const;
	bool operator<(const Variant &right) const;
	bool operator<=(const Variant &right) const;

	void operator=(const Variant &r);

	//Methods
	bool isdef() const;
	VariantType get_type() const;

	//Functions
	Array<String > ListMemberFuncNames();
	//bool FuncExists(const String  &name);
	//Variant RunFunc(const String  &name, Array<Variant> paras);

	//Variables
	//void set(const Variant& key, const Variant& val);
	//Variant get(const Variant& key);
	Array<String > ListMemberVars();
	//bool HasMember(const String  &name);

	//Array methods
	Variant GetElement(const Variant &index);
	void clear();
	void push_back(const Variant &var);

	int size();

	String ToString() const;

	//Conversions
	operator bool() const;
	operator int() const;
	operator float() const;
	operator double() const;
	operator Real() const;
	operator std::string() const { return operator String&(); }
	operator String() const { return operator String&(); }
	operator String&() const;
	operator vec2();
	operator vec3();
	operator vec4();
	operator mat4();
	operator Color();
	operator Transform();
	operator vec2&() const;
	operator vec3&() const;
	operator vec4&() const;
	operator mat4&() const;
	operator Color&() const;
	operator Transform&() const;
	operator Object*() const;

	//operator bool*() const { return b; }
	//operator int*() const { return &i; }
	//operator float*() const { return &f; }
	operator String*() const { return s; }
	operator vec2*() const { return v2; }
	operator vec3*() const { return v3; }
	operator vec4*() const { return v4; }
	operator mat4*() const { return m4; }
	operator Transform*() const { return t; }

	//best method imaginable
	template<typename T> operator T() const
	{
		if (isdef())
			return reinterpret_cast<T>(o);
		else
			return 0;
	}

	template<typename T>
	T get_derived_object() const
	{
		return dynamic_cast<T*>(o);
	}

	//get specific value
	template<typename T>
	T get_value() const
	{
		switch (type)
		{
		case BOOL:
			return b;
		// case REAL:
		// 	return *r;
		case STRING:
			return *s;
		case VEC2:
			return *v2;
		case VEC3:
			return *v3;
		case VEC4:
			return *v4;
		case MAT4:
			return *m4;
		case OBJECT:
			return *o;
		}
	}

	int type;

private:
	void convert_error(const String& p_type) const;

	bool is_ptr() const;

};

struct VariantPtr
{
	VariantPtr(Variant &v)
	{
		data = &v.b;
	}

protected:
	VariantPtr() = default;

	void *data;
};

#if 0

struct VariantPtrExt : VariantPtr
{
	VariantPtrExt(Variant &v) : VariantPtr(v)
	{
		type = (int) v.type;
	}

	VariantPtrExt(bool &p_b) { type = 1; data = &p_b; }
	VariantPtrExt(int &p_i) { type = 2; data = &p_i; }
	VariantPtrExt(int &p_i) { type = 2; data = &p_i; }
	VariantPtrExt(float &p_d) { type = 2; data = &p_d; }
	VariantPtrExt(Real &p_r) { type = 2; data = &p_r; }
	VariantPtrExt(String &p_s) { type = 3; data = &p_s; }
	VariantPtrExt(string &p_s) { type = 3; data = &p_s; }
	VariantPtrExt(vec2 &p_v2) { type = 4; data = &p_v2; }
	VariantPtrExt(vec3 &p_v3) { type = 5; data = &p_v3; }
	VariantPtrExt(vec4 &p_v4) { type = 6; data = &p_v4; }
	VariantPtrExt(mat4 &p_m4) { type = 7; data = &p_m4; }
	VariantPtrExt(Color &p_c) { type = 8; data = &p_c; }
	VariantPtrExt(Transform &p_t) { type = 9; data = &p_t; }
	VariantPtrExt(Object *p_c) { type = 11; data = p_c; }
	VariantPtrExt(Array<Variant> p_a) { type = 11; data = &p_a; }

	int type;
};


struct VariantRef
{
	VariantRef(bool &p_b) {	b = p_b; }
	VariantRef(Real &p_b) { r = p_b; }
	VariantRef(String *&p_b) { s = p_b; }
	VariantRef(vec2 &p_b) { v2 = p_b; }
	VariantRef(vec3 &p_b) { v3 = p_b; }
	VariantRef(vec4 &p_b) { v4 = p_b; }
	VariantRef(mat4 &p_b) { m4 = p_b; }
	VariantRef(Color &p_b) { c = p_b; }
	VariantRef(Transform &p_b) { t = p_b; }
	VariantRef(Object *&p_b) { o = p_b; }
	VariantRef(Array<Variant> &p_b) { a = p_b; }

	bool &b;
	Real &r;
	String *&s;
	vec2 &v2;
	vec3 &v3;
	vec4 &v4;
	mat4 &m4;
	Color &c;
	Transform &t;
	Object *&o;

	Array<Variant> &a;
};


struct VariantValue
{
#define GETVALUE(T)\
	static T get_value(T v)\
	{\
		return v;\
	}

	GETVALUE(bool)
	GETVALUE(Real)
	GETVALUE(String)
	GETVALUE(vec2)
	GETVALUE(vec3)
	GETVALUE(vec4)
	GETVALUE(mat4)
	GETVALUE(Object*)
};
#endif

// Helper classes to avoid the use of Variant::operator Variant() const.
template<typename T> struct VariantCaster
{
   static inline T get(Variant& v) { return v.operator T(); }
};

template<> struct VariantCaster<Variant>
{
   static inline Variant get(Variant& v) { return v; }
};

template<typename T> T cast(Variant& v )
{
   return VariantCaster<T>::get(v);
}
