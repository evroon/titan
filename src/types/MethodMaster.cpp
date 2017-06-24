#include "MethodMaster.h"

#include "Method.h"
#include "MethodBuilder.h"

#include "Rect.h"

MethodMaster *MethodMaster::method_master;

Method* ObjectCallables::get_method_by_name(const StringName &name)
{
	for (int c = 0; c < methods.size(); c++)
		if (methods[c]->name == name)
			return methods[c];

	return NULL;
}

Property* ObjectCallables::get_getsetter_by_name(const StringName &name)
{
	for (int c = 0; c < properties.size(); c++)
		if (properties[c]->var_name == name)
			return properties[c];

	return NULL;
}

TConstructor* ObjectCallables::get_constructor_by_params(int param_count)
{
	for (int c = 0; c < constructors.size(); c++)
		if (constructors[c]->arg_count == param_count)
			return constructors[c];

	return NULL;
}

void ObjectCallables::free()
{
	//methods.clean();
	//properties.clean();
	//constructors.clean();
}

//=========================================================================
//MethodMaster
//=========================================================================

void MethodMaster::init()
{
	method_master = new MethodMaster;

	method_master->add_fundamental_methods();
	method_master->add_static_functions();
	//method_master->add_inherited_methods();
}

void MethodMaster::add_inherited_methods()
{	
	for (std::pair<const StringName, ObjectType> &o : TypeManager::get_singleton()->object_types)
	{
		Array<String> a = o.second.path.split('/');

		// do not add the object's own methods again
		a.removelast();

		for (int c = 0; c < a.size(); c++)
		{
			for (Method *method : object_callables[VariantType(a[c])].methods)
				register_method(o.second.name, method);
			
			for (Property *property : object_callables[VariantType(a[c])].properties)
				register_property(o.second.name, property);
		}
	}
}

void MethodMaster::add_fundamental_methods()
{
	//register vec2
	TYPEMAN->add_type(StringName("vec2"));

	REG_CSTR_NO(vec2, 0);
	REG_CSTR_NO(vec2, 2);
	REG_CSTR_NO_OVRLD_1(vec2, float);

	REG_METHOD_NO(vec2, angle);
	REG_METHOD_NO(vec2, normalize);
	REG_METHOD_NO(vec2, length);
	REG_METHOD_NO(vec2, dot);
	REG_METHOD_NO(vec2, to_string);

	REG_PROPERTY_NO(vec2, x);
	REG_PROPERTY_NO(vec2, y);

	//register vec3
	TYPEMAN->add_type(StringName("vec3"));

	REG_CSTR_NO(vec3, 0);
	REG_CSTR_NO(vec3, 3);
	REG_CSTR_NO_OVRLD_1(vec3, float);
	REG_CSTR_NO_OVRLD_2(vec3, float, vec2);
	REG_CSTR_NO_OVRLD_2(vec3, vec2, float);

	REG_METHOD_NO(vec3, angle);
	REG_METHOD_NO(vec3, normalize);
	REG_METHOD_NO(vec3, length);
	REG_METHOD_NO(vec3, dot);
	REG_METHOD_NO(vec3, to_string);

	REG_PROPERTY_NO(vec3, x);
	REG_PROPERTY_NO(vec3, y);
	REG_PROPERTY_NO(vec3, z);
	
	//register vec4
	TYPEMAN->add_type(StringName("vec4"));

	REG_CSTR_NO(vec4, 0);
	REG_CSTR_NO(vec4, 4);
	REG_CSTR_NO_OVRLD_1(vec4, float);
	REG_CSTR_NO_OVRLD_2(vec4, vec2, vec2);
	REG_CSTR_NO_OVRLD_2(vec4, vec3, float);
	REG_CSTR_NO_OVRLD_2(vec4, float, vec3);
	REG_CSTR_NO_OVRLD_3(vec4, vec2, float, float);
	REG_CSTR_NO_OVRLD_3(vec4, float, vec2, float);
	REG_CSTR_NO_OVRLD_3(vec4, float, float, vec2);

	REG_METHOD_NO(vec4, angle);
	REG_METHOD_NO(vec4, normalize);
	REG_METHOD_NO(vec4, length);
	REG_METHOD_NO(vec4, dot);
	REG_METHOD_NO(vec4, to_string);

	REG_PROPERTY_NO(vec4, x);
	REG_PROPERTY_NO(vec4, y);
	REG_PROPERTY_NO(vec4, z);
	REG_PROPERTY_NO(vec4, w);

	//register String
	TYPEMAN->add_type(StringName("String"));

	REG_CSTR_FULL(String, "String", 0);

	//register Transform
	TYPEMAN->add_type(StringName("Transform"));

	REG_CSTR_NO(Transform, 0);
	REG_CSTR_NO_OVRLD_1(Transform, vec3);
	REG_CSTR_NO_OVRLD_2(Transform, vec2, vec2);
	REG_CSTR_NO_OVRLD_2(Transform, vec3, vec2);
	REG_CSTR_NO_OVRLD_2(Transform, vec2, vec3);
	REG_CSTR_NO_OVRLD_2(Transform, vec3, vec3);
	REG_CSTR_NO_OVRLD_3(Transform, vec2, vec2, float);
	REG_CSTR_NO_OVRLD_3(Transform, vec3, vec2, float);
	REG_CSTR_NO_OVRLD_3(Transform, vec2, vec3, float);
	REG_CSTR_NO_OVRLD_3(Transform, vec3, vec3, float);

	REG_PROPERTY_NO(Transform, pos);
	REG_PROPERTY_NO(Transform, size);
	REG_PROPERTY_NO(Transform, rotation);

	//register rect2
	/*TYPEMAN->add_type(StringName("rect2"));

	REG_CSTR_NO(rect2, 0);
	REG_CSTR_NO_OVRLD_1(Transform, vec2);
	REG_CSTR_NO(rect2, 2);
	REG_CSTR_NO(rect2, 4);

	REG_PROPERTY_NO(rect2, pos);
	REG_PROPERTY_NO(rect2, size);*/

	
}

void MethodMaster::add_static_functions()
{
	//register_static_func(StringName("sin"), new R_Method_1([](float f) { return ::sin(f); }));
	//register_static_func(StringName("sgn"), new R_Method_1([](Variant f) { return Math::sgn<float>(f); }));
	//register_static_func(StringName("print"), new V_Method_1([](const String &s) { new TLog(s); }));

	//REG_STATIC_FUNC("sin", Math::sgn<float>);
}

void MethodMaster::register_constant(VariantType type, const ConstantMember &p_constant)
{
	object_callables[type].constants.push_back(p_constant);
}

void MethodMaster::register_singleton(VariantType type, Variant p_singleton)
{
	object_callables[type].singleton = p_singleton;
}

//register
void MethodMaster::register_method(VariantType type, Method *method)
{
	object_callables[type].methods.push_back(method);
}

void MethodMaster::register_property(VariantType type, Property *getset)
{
	object_callables[type].properties.push_back(getset);
}

void MethodMaster::register_constructor(VariantType type, TConstructor *cstr)
{
	object_callables[type].constructors.push_back(cstr);
}

void MethodMaster::register_static_func(StringName name, Method *method)
{
	static_funcs[name] = method;
}

void MethodMaster::register_signal(const VariantType &p_type, const StringName &p_signal)
{
	object_callables[p_type].signal_names.push_back(p_signal);
}

//does exist
bool MethodMaster::method_exists(VariantType type, const StringName &name)
{
	if (!object_callables.contains(type))
		return false;

	for (Method *m : object_callables[type].methods)
		if (m->name == name)
			return true;

	return false;
}
bool MethodMaster::property_exists(VariantType type, const StringName &name)
{
	if (!object_callables.contains(type))
		return false;

	for (Property *gs : object_callables[type].properties)
		if (gs->var_name == name)
			return true;

	return false;
}
bool MethodMaster::constructor_exists(VariantType type, int argc)
{
	if (!object_callables.contains(type))
		return false;

	for (TConstructor * m : object_callables[type].constructors)
		if (m->arg_count == argc)
			return true;

	return false;
}

bool MethodMaster::signal_exists(const VariantType & p_type, const StringName & p_signal)
{
	if (!object_callables.contains(p_type))
		return false;


	Array<String> a = TYPEMAN->get_object_type(p_type.get_type_name()).path.split('/');

	for (int c = 0; c < a.size(); c++)
	{
		if (object_callables[VariantType(a[c])].signal_names.contains(p_signal))
			return true;
	}

	return false;
}

//get
Method* MethodMaster::get_method(VariantType type, const StringName &name)
{
	if (object_callables.contains(type))
		return object_callables[type].get_method_by_name(name);
	else
		return NULL;
}
Property* MethodMaster::get_property(VariantType type, const StringName &name)
{
	if (object_callables.contains(type))
		return object_callables[type].get_getsetter_by_name(name);
	else
		return NULL;
}
TConstructor* MethodMaster::get_constructor(VariantType type, int param_count)
{
	if (object_callables.contains(type))
		return object_callables[type].get_constructor_by_params(param_count);
	else
		return NULL;
}

Variant MethodMaster::get_singleton(VariantType p_type)
{
	if (object_callables.contains(p_type))
		return object_callables[p_type].singleton;
	else
		return NULL;
}

void MethodMaster::clean()
{
	for (std::pair<const int, ObjectCallables> &oc : MMASTER->object_callables)
		oc.second.free();
}

MethodMaster* MethodMaster::get_method_master()
{
	return method_master;
}

Array<StringName> MethodMaster::list_method_names(VariantType type)
{
	if (!type.is_def())
		return Array<StringName>();

	Array<StringName> result;

	for (Method *m : object_callables[type].methods)
		result.push_back(m->name);

	return result;
}

Array<StringName> MethodMaster::list_property_names(VariantType type)
{
	if (!type.is_def())
		return Array<StringName>();

	Array<StringName> result;

	for (Property *m : object_callables[type].properties)
		result.push_back(m->var_name);

	return result;
}

Array<StringName> MethodMaster::list_constructor_names(VariantType type)
{
	if (!type.is_def())
		return Array<StringName>();

	Array<StringName> result;

	//for (TConstructor *m : object_callables[type].constructors)
	//	result.push_back(m->name);

	return result;
}