#pragma once

#include "MethodMaster.h"

#define CLASSNAME

//build functions referring to functions below

//register constructor
#define REG_CSTR_FULL(TYPE, TYPENAME, ARGC) \
	MethodBuilder::register_constructor(\
		ConstructorBuilder::build_##ARGC<TYPE>(), ParameterNames(), VariantType(StringName(TYPENAME)))

#define REG_CSTR(ARGC) \
	REG_CSTR_FULL(CLASSNAME, CLASSNAME::get_type_name_static().get_source(), ARGC)

#define REG_CSTR_NO(TYPE, ARGC) \
	REG_CSTR_FULL(TYPE, #TYPE, ARGC)

//overloaded constructor
#define REG_CSTR_FULL_OVRLD_1(TYPE, TYPENAME, ARG_0) \
	MethodBuilder::register_constructor(\
		ConstructorBuilder::build_1_overloaded<TYPE, ARG_0>(), ParameterNames(), VariantType(StringName(TYPENAME)))

#define REG_CSTR_FULL_OVRLD_2(TYPE, TYPENAME, ARG_0, ARG_1) \
	MethodBuilder::register_constructor(\
		ConstructorBuilder::build_2_overloaded<TYPE, ARG_0, ARG_1>(), ParameterNames(), VariantType(StringName(TYPENAME)))

#define REG_CSTR_FULL_OVRLD_3(TYPE, TYPENAME, ARG_0, ARG_1, ARG_2) \
	MethodBuilder::register_constructor(\
		ConstructorBuilder::build_3_overloaded<TYPE, ARG_0, ARG_1, ARG_2>(), ParameterNames(), VariantType(StringName(TYPENAME)))

#define REG_CSTR_FULL_OVRLD_4(TYPE, TYPENAME, ARG_0, ARG_1, ARG_2, ARG_3) \
	MethodBuilder::register_constructor(\
		ConstructorBuilder::build_4_overloaded<TYPE, ARG_0, ARG_1, ARG_2, ARG_3>(), ParameterNames(), VariantType(StringName(TYPENAME)))

#define REG_CSTR_OVRLD_1(ARG_0) \
	REG_CSTR_FULL_OVRLD_1(CLASSNAME, CLASSNAME::get_type_name_static().get_source(), ARG_0)

#define REG_CSTR_OVRLD_2(ARG_0, ARG_1) \
	REG_CSTR_FULL_OVRLD_2(CLASSNAME, CLASSNAME::get_type_name_static().get_source(), ARG_0, ARG_1)

#define REG_CSTR_OVRLD_3(ARG_0, ARG_1, ARG_2) \
	REG_CSTR_FULL_OVRLD_3(CLASSNAME, CLASSNAME::get_type_name_static().get_source(), ARG_0, ARG_1, ARG_2)

#define REG_CSTR_OVRLD_4(ARG_0, ARG_1, ARG_2, ARG_3) \
	REG_CSTR_FULL_OVRLD_4(CLASSNAME, CLASSNAME::get_type_name_static().get_source(), ARG_0, ARG_1, ARG_2, ARG_3)

#define REG_CSTR_NO_OVRLD_1(TYPE, ARG_0) \
	REG_CSTR_FULL_OVRLD_1(TYPE, #TYPE, ARG_0)

#define REG_CSTR_NO_OVRLD_2(TYPE, ARG_0, ARG_1) \
	REG_CSTR_FULL_OVRLD_2(TYPE, #TYPE, ARG_0, ARG_1)

#define REG_CSTR_NO_OVRLD_3(TYPE, ARG_0, ARG_1, ARG_2) \
	REG_CSTR_FULL_OVRLD_3(TYPE, #TYPE, ARG_0, ARG_1, ARG_2)

#define REG_CSTR_NO_OVRLD_4(TYPE, ARG_0, ARG_1, ARG_2, ARG_3) \
	REG_CSTR_FULL_OVRLD_4(TYPE, #TYPE, ARG_0, ARG_1, ARG_2, ARG_3)


//register method
#define REG_METHOD_FULL(TYPE, TYPENAME, METHOD) \
	MethodBuilder::reg_method( \
		FunctorBuilder::build(MethodBinder::bind(&TYPE::METHOD)), \
		StringName(#METHOD), \
		ParameterNames(), \
		VariantType(StringName(TYPENAME)))

#define REG_METHOD(METHOD) \
	REG_METHOD_FULL(CLASSNAME, CLASSNAME::get_type_name_static().get_source(), METHOD)

#define REG_METHOD_NO(TYPE, METHOD) \
	REG_METHOD_FULL(TYPE, #TYPE, METHOD)

//register property (with getter and setter)
#define REG_PROPERTY_FULL(TYPE, TYPENAME, NAME) \
	MethodBuilder::register_property( \
		REG_METHOD_FULL(TYPE, TYPENAME, get_##NAME), \
		REG_METHOD_FULL(TYPE, TYPENAME, set_##NAME), \
		StringName(#NAME), \
		VariantType(StringName(TYPENAME)))

#define REG_PROPERTY(NAME) \
	MethodBuilder::register_property( \
		REG_METHOD(get_##NAME), \
		REG_METHOD(set_##NAME), \
		StringName(#NAME), \
		VariantType(StringName(CLASSNAME::get_type_name_static().get_source())))

#define REG_PROPERTY_NO(TYPE, NAME)\
	REG_PROPERTY_FULL(TYPE, #TYPE, NAME)

//register static function
#define REG_STATIC_FUNC(NAME, METHOD) \
	MethodBuilder::reg_static_func( \
		FunctorBuilder::build(METHOD), \
		StringName(NAME), \
		ParameterNames())

#undef VAR
#define VAR Variant

struct FunctorBuilder
{
	//build a functor for a returning function
	template<typename T>
	static std::function<VAR(VAR)> build(std::function<VAR(T)> f)
	{
		return [f](VAR v) { return f(v); };
	}

	template<typename T, typename ARG_0>
	static std::function<VAR(VAR, VAR)> build(std::function<VAR(T, ARG_0)> f)
	{
		return [f](VAR v, VAR arg_0) { return f(v, arg_0); };
	}

	template<typename T, typename ARG_0, typename ARG_1>
	static std::function<VAR(VAR, VAR, VAR)> build(std::function<VAR(T, ARG_0, ARG_1)> f)
	{
		return [f](VAR v, VAR arg_0, VAR arg_1) { return f(v, arg_0, arg_1); };
	}

	//build a functor for a void function
	template<typename T>
	static std::function<void(VAR)> build(std::function<void(T)> f)
	{
		return [f](VAR v) { f(v); };
	}

	template<typename T, typename ARG_0>
	static std::function<void(VAR, VAR)> build(std::function<void(T, ARG_0)> f)
	{
		return [f](VAR v, VAR arg_0) { f(v, arg_0); };
	}
	
	template<typename T, typename ARG_0, typename ARG_1>
	static std::function<void(VAR, VAR, VAR)> build(std::function<void(T, ARG_0, ARG_1)> f)
	{
		return [f](VAR v, VAR arg_0, VAR arg_1) { f(v, arg_0, arg_1); };
	}
};

struct ConstructorBuilder
{
	template<typename T>
	static std::function<Variant()> build_0()
	{
		return []() { return new T(); };
	}
	template<typename T>
	static std::function<Variant(Variant)> build_1()
	{
		return [](Variant arg_0) { return new T(arg_0); };
	}
	template<typename T>
	static std::function<Variant(Variant, Variant)> build_2()
	{
		return [](Variant arg_0, Variant arg_1) { return new T(arg_0, arg_1); };
	}
	template<typename T>
	static std::function<Variant(Variant, Variant, Variant)> build_3()
	{
		return [](Variant arg_0, Variant arg_1, Variant arg_2) { return new T(arg_0, arg_1, arg_2); };
	}
	template<typename T>
	static std::function<Variant(Variant, Variant, Variant, Variant)> build_4()
	{
		return [](Variant arg_0, Variant arg_1, Variant arg_2, Variant arg_3) { return new T(arg_0, arg_1, arg_2, arg_3); };
	}

	//=========================================================================
	//OVERLOADED
	//=========================================================================

	template<typename T, typename ARG_0>
	static std::function<Variant(Variant)> build_1_overloaded()
	{
		return [](Variant arg_0) { return new T(arg_0.operator ARG_0()); };
	}
	template<typename T, typename ARG_0, typename ARG_1>
	static std::function<Variant(Variant, Variant)> build_2_overloaded()
	{
		return [](Variant arg_0, Variant arg_1) { return new T(arg_0.operator ARG_0(), arg_1.operator ARG_1()); };
	}
	template<typename T, typename ARG_0, typename ARG_1, typename ARG_2>
	static std::function<Variant(Variant, Variant, Variant)> build_3_overloaded()
	{
		return [](Variant arg_0, Variant arg_1, Variant arg_2) { return new T(arg_0.operator ARG_0(), arg_1.operator ARG_1(), arg_2.operator ARG_2()); };
	}
	template<typename T, typename ARG_0, typename ARG_1, typename ARG_2, typename ARG_3>
	static std::function<Variant(Variant, Variant, Variant, Variant)> build_4_overloaded()
	{
		return [](Variant arg_0, Variant arg_1, Variant arg_2, Variant arg_3) { return new T(arg_0.operator ARG_0(), arg_1.operator ARG_1(), arg_2.operator ARG_2(), arg_3.operator ARG_3()); };
	}
};

struct MethodBinder
{
	//=========================================================================
	//OBJECT: NON-CONST (WITH RETURN)
	//=========================================================================

	template<typename R, typename T>
	static std::function<VAR(T*)> bind(R(T::*f) ())
	{
		get_singleton()->arg_typenames.clear();
		get_singleton()->return_type = GetType<R>();
		get_singleton()->is_const = false;

		return std::bind(f, std::placeholders::_1);
	}

	template<typename R, typename T, typename A_0>
	static std::function<VAR(T*, A_0)> bind(R(T::*f) (A_0))
	{
		get_singleton()->arg_typenames.clear();
		get_singleton()->arg_typenames.push_back(GetType<A_0>());
		get_singleton()->return_type = GetType<R>();
		get_singleton()->is_const = false;

		return std::bind(f, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename R, typename T, typename A_0, typename A_1>
	static std::function<VAR(T*, A_0, A_1)> bind(R(T::*f) (A_0, A_1))
	{
		get_singleton()->arg_typenames.clear();
		get_singleton()->arg_typenames.push_back(GetType<A_0>());
		get_singleton()->arg_typenames.push_back(GetType<A_1>());
		get_singleton()->return_type = GetType<R>();
		get_singleton()->is_const = false;

		return std::bind(f, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	//=========================================================================
	//OBJECT: NON-CONST (VOID)
	//=========================================================================

	template<typename T>
	static std::function<void(T*)> bind(void(T::*f) ())
	{
		get_singleton()->arg_typenames.clear();
		get_singleton()->is_const = false;

		return std::bind(f, std::placeholders::_1);
	}

	template<typename T, typename A_0>
	static std::function<void(T*, A_0)> bind(void(T::*f) (A_0))
	{
		get_singleton()->arg_typenames.clear();
		get_singleton()->arg_typenames.push_back(GetType<A_0>());
		get_singleton()->is_const = false;

		return std::bind(f, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename T, typename A_0, typename A_1>
	static std::function<void(T*, A_0, A_1)> bind(void(T::*f) (A_0, A_1))
	{
		get_singleton()->arg_typenames.clear();
		get_singleton()->arg_typenames.push_back(GetType<A_0>());
		get_singleton()->arg_typenames.push_back(GetType<A_1>());
		get_singleton()->is_const = false;

		return std::bind(f, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	//=========================================================================
	//OBJECT: CONST (WITH RETURN)
	//=========================================================================

	template<typename R, typename T>
	static std::function<VAR(T*)> bind(R(T::*f) () const)
	{
		get_singleton()->arg_typenames.clear();
		get_singleton()->return_type = GetType<R>();
		get_singleton()->is_const = true;

		return std::bind(f, std::placeholders::_1);
	}

	template<typename R, typename T, typename A_0>
	static std::function<VAR(T*, A_0)> bind(R(T::*f) (A_0) const)
	{
		get_singleton()->arg_typenames.clear();
		get_singleton()->arg_typenames.push_back(GetType<A_0>());
		get_singleton()->return_type = GetType<R>();
		get_singleton()->is_const = true;

		return std::bind(f, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename R, typename T, typename A_0, typename A_1>
	static std::function<VAR(T*, A_0, A_1)> bind(R(T::*f) (A_0, A_1) const)
	{
		get_singleton()->arg_typenames.clear();
		get_singleton()->arg_typenames.push_back(GetType<A_0>());
		get_singleton()->arg_typenames.push_back(GetType<A_1>());
		get_singleton()->return_type = GetType<R>();
		get_singleton()->is_const = true;

		return std::bind(f, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	//=========================================================================
	//OBJECT: CONST (VOID)
	//=========================================================================

	template<typename T>
	static std::function<void(T*)> bind(void(T::*f) () const)
	{
		get_singleton()->arg_typenames.clear();
		get_singleton()->is_const = true;

		return std::bind(f, std::placeholders::_1);
	}

	template<typename T, typename A_0>
	static std::function<void(T*, A_0)> bind(void(T::*f) (A_0) const)
	{
		get_singleton()->arg_typenames.clear();
		get_singleton()->arg_typenames.push_back(GetType<A_0>());
		get_singleton()->is_const = true;

		return std::bind(f, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename T, typename A_0, typename A_1>
	static std::function<void(T*, A_0, A_1)> bind(void(T::*f) (A_0, A_1) const)
	{
		get_singleton()->arg_typenames.clear();
		get_singleton()->arg_typenames.push_back(GetType<A_0>());
		get_singleton()->arg_typenames.push_back(GetType<A_1>());
		get_singleton()->is_const = true;

		return std::bind(f, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	static MethodBinder* get_singleton()
	{
		static MethodBinder mb;
		return &mb;
	}

	//metadata of last bounded function
	bool is_const;
	ParameterTypes arg_typenames;
	String return_type;
};

class MethodBuilder
{
public:
#define ARG_ARRAY ParameterNames p_args
#define VAR_TYPE VariantType var_type

	//void method
	static V_Method_1* reg_method(std::function<void(VAR)> p_func, const StringName &name, const ParameterNames &p_args, VAR_TYPE);
	static V_Method_2* reg_method(std::function<void(VAR, VAR)> p_func, const StringName &name, const ParameterNames &p_args, VAR_TYPE);
	static V_Method_3* reg_method(std::function<void(VAR, VAR, VAR)> p_func, const StringName &name, const ParameterNames &p_args, VAR_TYPE);

	//return method
	static R_Method_1* reg_method(std::function<VAR(VAR)> p_func, const StringName &name, const ParameterNames &p_args, VAR_TYPE);
	static R_Method_2* reg_method(std::function<VAR(VAR, VAR)> p_func, const StringName &name, const ParameterNames &p_args, VAR_TYPE);
	static R_Method_3* reg_method(std::function<VAR(VAR, VAR, VAR)> p_func, const StringName &name, const ParameterNames &p_args, VAR_TYPE);

	//static void method
	static V_Method_0* reg_static_func(std::function<void()> p_func, const StringName &name, const ParameterNames &p_args);
	static V_Method_1* reg_static_func(std::function<void(VAR)> p_func, const StringName &name, const ParameterNames &p_args);
	static V_Method_2* reg_static_func(std::function<void(VAR, VAR)> p_func, const StringName &name, const ParameterNames &p_args);

	//static return method
	static R_Method_0* reg_static_func(std::function<VAR()> p_func, const StringName &name, const ParameterNames &p_args);
	static R_Method_1* reg_static_func(std::function<VAR(VAR)> p_func, const StringName &name, const ParameterNames &p_args);
	static R_Method_2* reg_static_func(std::function<VAR(VAR, VAR)> p_func, const StringName &name, const ParameterNames &p_args);
	
	//constructor
	static CSTR_0* register_constructor(std::function<VAR()> p_func, const ParameterNames &p_args, VAR_TYPE);
	static CSTR_1* register_constructor(std::function<VAR(VAR)> p_func, const ParameterNames &p_args, VAR_TYPE);
	static CSTR_2* register_constructor(std::function<VAR(VAR, VAR)> p_func, const ParameterNames &p_args, VAR_TYPE);
	static CSTR_3* register_constructor(std::function<VAR(VAR, VAR, VAR)> p_func, const ParameterNames &p_args, VAR_TYPE);
	static CSTR_4* register_constructor(std::function<VAR(VAR, VAR, VAR, VAR)> p_func, const ParameterNames &p_args, VAR_TYPE);

	// Property
	static Property* register_property(R_Method_1 *p_get, V_Method_2 *p_set, const StringName &name, VAR_TYPE);
};