#pragma once

#include <string>

#include "types/typemanager.h"
#include "variant.h"
#include "core/corenames.h"

class VariantType
{
public:
	VariantType() : VariantType(StringName(CORE_TYPE(Null))) { }
	VariantType(int type) : VariantType(static_cast<Variant::Type>(type)){ }
	VariantType(Variant::Type type);
	VariantType(const String &name) : VariantType(StringName(name)) { }
	VariantType(const StringName &name);
	VariantType(ObjectTypeRef p_object_type);

	StringName get_type_name() const;

	bool is_object_type() const;

	void set_object_type(ObjectTypeRef p_object_type);

	void set_type(const StringName &name);

	bool is_def() const;

	template<typename T>
	bool is_of_type() const
	{
		return GETNAME(T::get_type_ptr_static()) == type_name;
	}

	template<typename T>
	bool derives_from_type() const
	{
		return TYPEMAN->get_object_type(type_name).path.Contains(T::get_type_name_static().get_source());
	}

	//operators
	bool operator==(const VariantType &p_var_type) const;
	bool operator==(const ObjectTypeRef &p_object_type) const;

	operator int() const;
	operator Variant::Type() const;

	static bool is_object_type(const StringName &name);


private:
	String get_object_typename() const;
	StringName type_name;
};
