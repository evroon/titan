#include "varianttype.h"

#include "core/corenames.h"

VariantType::VariantType(Variant::Type type) : VariantType()
{
	switch (type)
	{
	case Variant::UNDEF:
		type_name = CORE_TYPE(Null);
		break;

	case Variant::BOOL:
		type_name = CORE_TYPE(Bool);
		break;

	case Variant::INT:
		type_name = CORE_TYPE(Int);
		break;

	case Variant::FLOAT:
		type_name = CORE_TYPE(Float);
		break;

	case Variant::STRING:
		type_name = CORE_TYPE(String);
		break;

	case Variant::VEC2:
		type_name = CORE_TYPE(vec2);
		break;

	case Variant::VEC3:
		type_name = CORE_TYPE(vec3);
		break;

	case Variant::VEC4:
		type_name = CORE_TYPE(vec4);
		break;

	case Variant::MAT4:
		type_name = CORE_TYPE(mat4);
		break;

	case Variant::COLOR:
		type_name = CORE_TYPE(Color);
		break;

	case Variant::TRANSFORM:
		type_name = CORE_TYPE(Transform);
		break;

	case Variant::OBJECT:
		type_name = CORE_TYPE(Object);
		break;

	case Variant::ARRAY:
		type_name = CORE_TYPE(Array);
		break;
	}
}

VariantType::VariantType(const StringName &name) : type_name(name)
{

}

VariantType::VariantType(ObjectTypeRef p_object_type) : type_name(p_object_type->name)
{

}

String VariantType::get_object_typename() const
{
	return type_name.get_source();
}

StringName VariantType::get_type_name() const
{
	return type_name;
}

bool VariantType::is_object_type(const StringName &name)
{
	return
		name != CORE_TYPE(Null) &&
		name != CORE_TYPE(Bool) &&
		name != CORE_TYPE(Real) &&
		name != CORE_TYPE(String) &&
		name != CORE_TYPE(vec2) &&
		name != CORE_TYPE(vec3) &&
		name != CORE_TYPE(vec4) &&
		name != CORE_TYPE(mat4) &&
		name != CORE_TYPE(Color) &&
		name != CORE_TYPE(Transform) &&
		name != CORE_TYPE(Array);
}

bool VariantType::is_object_type() const
{
	return VariantType::is_object_type(type_name);
}

void VariantType::set_object_type(ObjectTypeRef p_object_type)
{
	type_name = p_object_type->name;
}

void VariantType::set_type(const StringName &name)
{
	type_name = name;
}

bool VariantType::is_def() const
{
	return type_name != CORE_TYPE(Null);
}

//operators
bool VariantType::operator==(const VariantType &p_var_type) const
{
	return type_name == p_var_type.type_name;
}
bool VariantType::operator==(const ObjectTypeRef &p_object_type) const
{
	return type_name == p_object_type->name;
}

VariantType::operator int() const
{
	return to_int(type_name);
}

VariantType::operator Variant::Type() const
{
	if (type_name == CORE_TYPE(Null))
		return Variant::UNDEF;
	else if (type_name == CORE_TYPE(Bool))
		return Variant::BOOL;
	else if (type_name == CORE_TYPE(Int))
		return Variant::INT;
	else if (type_name == CORE_TYPE(Float))
		return Variant::FLOAT;
	else if (type_name == CORE_TYPE(String))
		return Variant::STRING;
	else if (type_name == CORE_TYPE(vec2))
		return Variant::VEC2;
	else if (type_name == CORE_TYPE(vec3))
		return Variant::VEC3;
	else if (type_name == CORE_TYPE(vec4))
		return Variant::VEC4;
	else if (type_name == CORE_TYPE(mat4))
		return Variant::MAT4;
	else if (type_name == CORE_TYPE(Color))
		return Variant::COLOR;
	else if (type_name == CORE_TYPE(Transform))
		return Variant::TRANSFORM;
	else if (type_name == CORE_TYPE(Array))
		return Variant::ARRAY;
	else
		return Variant::OBJECT;

}
