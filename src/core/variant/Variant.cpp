#include "Variant.h"

#include "core/Memory.h"

#include "core/Object.h"
#include "input/Key.h"
#include "VariantType.h"

Variant::Variant()
{
	type = UNDEF;
}

//Construct with value
Variant::Variant(const Variant &r)
{
	reference(r);
}
Variant::Variant(const VariantPtrExt &p)
{
	//type = p.type;
	//ptr = p.data;
}
Variant::Variant(unsigned p_i)
{
	i = p_i;
	type = INT;
}
Variant::Variant(int p_i)
{
	i = p_i;
	type = INT;
}
Variant::Variant(float p_f)
{
	f = p_f;
	type = FLOAT;
}
Variant::Variant(const Real &p_r)
{
	if (p_r.t == Real::INT)
	{
		i = p_r.i;
		type = INT;
	}
	else
	{
		f = p_r.f;
		type = FLOAT;
	}
}
Variant::Variant(bool p_b)
{
	b = p_b;
	type = BOOL;
}
Variant::Variant(const String &p_s)
{
	s = new String(p_s);
	type = STRING;
	//GC->queue(*this);
}
Variant::Variant(const vec2 &p_v2)
{
	v2 = new vec2(p_v2);
	type = VEC2;
}
Variant::Variant(const vec3 &p_v3)
{
	v3 = new vec3(p_v3);
	type = VEC3;
}
Variant::Variant(const vec4 &p_v4)
{
	v4 = new vec4(p_v4);
	type = VEC4;
}
Variant::Variant(const mat4 &p_m4)
{
	m4 = new mat4(p_m4);
	type = MAT4;
}
Variant::Variant(const Color &p_c)
{
	c = new Color(p_c);
	type = COLOR;
}
Variant::Variant(const Transform &p_t)
{
	t = new Transform(p_t);
	type = TRANSFORM;
	//GC->queue(*this);
}
Variant::Variant(const Array<Variant> &p_a)
{
	if (p_a.size() == 0)
		type = UNDEF;
	else if (p_a.size() == 1)
		copy(p_a[0]);
	else
	{
		a = new Array<Variant>(p_a);
		type = ARRAY;
	}
}

//Construct with pointer
Variant::Variant(Object *p_g)
{
	o = p_g;

	if (!o)
		type = UNDEF;
	else
		type = OBJECT;
}
Variant::Variant(Real *p_r)
{
	if (p_r->t == Real::INT)
		i = p_r->i;
	else
		f = p_r->f;

	type = p_r->t == Real::INT ? INT : FLOAT;
}
Variant::Variant(String *p_s)
{
	s = p_s;
	type = STRING;
}
Variant::Variant(vec2 *p_v2)
{
	v2 = p_v2;
	type = VEC2;
}
Variant::Variant(vec3 *p_v3)
{
	v3 = p_v3;
	type = VEC3;
}
Variant::Variant(vec4 *p_v4)
{
	v4 = p_v4;
	type = VEC4;
}
Variant::Variant(mat4 *p_m4)
{
	m4 = p_m4;
	type = MAT4;
}
Variant::Variant(Color *p_c)
{
	c = p_c;
	type = COLOR;
}
Variant::Variant(Transform *p_t)
{
	t = p_t;
	type = TRANSFORM;
}

Variant::Variant(Array<Variant>* p_a)
{
	a = p_a;
	type = ARRAY;
}

Variant Variant::reference()
{
	Variant v;
	v.reference(*this);
	return v;
}

Variant Variant::copy()
{
	Variant v;
	v.copy(*this);
	return v;
}

void Variant::clean()
{
	switch (type)
	{
		case STRING:
			delete s;
			break;
		case TRANSFORM:
			break;
		case ARRAY:
			a->clear();
			break;
		case VEC2:
			delete v2;
			break;

		case OBJECT:

			break;
	}

	type = UNDEF;
}

void Variant::free()
{
	switch (type)
	{
	case UNDEF:
	case BOOL:
	case INT:
	case FLOAT:
		break;
	case STRING:
		delete s;
		break;
	case VEC2:
		delete v2;
		break;
	case VEC3:
		delete v3;
		break;
	case VEC4:
		delete v4;
		break;
	case MAT4:
		delete m4;
		break;
	case COLOR:
		delete c;
		break;
	case TRANSFORM:
		delete t;
		break;
	case ARRAY:
		delete a;
		break;
	case OBJECT:
		delete o;
		break;
	default:
		T_ERROR("Copy Error");
	}

	type = UNDEF;
}

void Variant::copy(const Variant &ref)
{
	type = ref.type;

	switch (type)
	{
		case UNDEF:
			break;
		case BOOL:
			b = ref.b;
			break;
		case INT:
			i = ref.i;
			break;
		case FLOAT:
			f = ref.f;
			break;
		case STRING:
			s = new String(*ref.s);
			break;
		case VEC2:
			v2 = new vec2(*ref.v2);
			break;
		case VEC3:
			v3 = new vec3(*ref.v3);
			break;
		case VEC4:
			v4 = new vec4(*ref.v4);
			break;
		case MAT4:
			m4 = new mat4(*ref.m4);
			break;
		case COLOR:
			c = new Color(*ref.c);
			break;
		case TRANSFORM:
			t = new Transform(*ref.t);
			break;
		case ARRAY:
			if (ref.a->size() == 0)
				type = UNDEF;
			else if (ref.a->size() == 1)
				copy(ref.a[0]);
			else
				a = ref.a;
			break;
		case OBJECT:
			o = ref.o;
			break;
		default:
			T_ERROR("Copy Error");
	}
}

void Variant::reference(const Variant &ref)
{
	type = ref.type;

	switch (type)
	{
		case UNDEF:
			break;
		case BOOL:
			b = ref.b;
			break;
		case INT:
			i = ref.i;
			break;
		case FLOAT:
			f = ref.f;
			break;
		case STRING:
			s = ref.s;
			break;
		case VEC2:
			v2 = ref.v2;
			break;
		case VEC3:
			v3 = ref.v3;
			break;
		case VEC4:
			v4 = ref.v4;
			break;
		case MAT4:
			m4 = ref.m4;
			break;
		case COLOR:
			c = ref.c;
			break;
		case TRANSFORM:
			t = ref.t;
			break;
		case ARRAY:
			a = ref.a;
			break;
		case OBJECT:
			o = ref.o;
			break;
		default:
			T_ERROR("Reference Error");
	}
}

bool Variant::isdef() const
{
	return type > UNDEF;
}

VariantType Variant::get_type() const
{
	if (type == OBJECT)
		return o->get_type();
	else return (Type) type;
}

bool Variant::is_ptr() const
{
	return type != BOOL && type != INT && type != FLOAT;
}
