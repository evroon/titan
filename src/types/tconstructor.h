#pragma once

#include <functional>

#include "callable.h"
#include "core/array.h"
#include "core/variant/variant.h"

//build constructors for Objects (as pointer)
#define BUILD_OBJ_CSTR_0() static Variant CLASSNAME##_0() \
	{ return new CLASSNAME(); }

#define BUILD_OBJ_CSTR_1() static Variant CLASSNAME##_1(VAR arg_0) \
	{ return new CLASSNAME(arg_0); }

#define BUILD_OBJ_CSTR_2() static Variant CLASSNAME##_2(VAR arg_0, VAR arg_1) \
	{ return new CLASSNAME(arg_0 COMMA arg_1); }

#define BUILD_OBJ_CSTR_3(TYPE) static Variant CLASSNAME##_3(VAR arg_0, VAR arg_1, VAR arg_2) \
	{ return new CLASSNAME(arg_0 COMMA arg_1 COMMA arg_2); }


struct TConstructor : Callable
{
	int arg_count;
	ParameterNames params;
};


struct CSTR_0 : public TConstructor
{
	CSTR_0() { arg_count = 0; }
	CSTR_0(std::function<Variant()> p_func) : CSTR_0() { func = p_func; }
	Variant operator()() { return func(); }
	std::function<Variant()> func;
};

struct CSTR_1 : public TConstructor
{
	CSTR_1() { arg_count = 1; }
	CSTR_1(std::function<Variant(Variant&)> p_func) : CSTR_1() { func = p_func; }
	Variant operator()(Variant arg_0) { return func(arg_0); }
	std::function<Variant(Variant&)> func;
};

struct CSTR_2 : public TConstructor
{
	CSTR_2() { arg_count = 2; }
	CSTR_2(std::function<Variant(Variant&, Variant&)> p_func) : CSTR_2() { func = p_func; }
	Variant operator()(Variant arg_0, Variant arg_1) { return func(arg_0, arg_1); }
	std::function<Variant(Variant&, Variant&)> func;
};

struct CSTR_3 : public TConstructor
{
	CSTR_3() { arg_count = 3; }
	CSTR_3(std::function<Variant(Variant&, Variant&, Variant&)> p_func) : CSTR_3() { func = p_func; }
	Variant operator()(Variant arg_0, Variant arg_1, Variant arg_2) { return func(arg_0, arg_1, arg_2); }
	std::function<Variant(Variant&, Variant&, Variant&)> func;
};

struct CSTR_4 : public TConstructor
{
	CSTR_4() { arg_count = 4; }
	CSTR_4(std::function<Variant(Variant&, Variant&, Variant&, Variant&)> p_func) : CSTR_4() { func = p_func; }
	Variant operator()(Variant arg_0, Variant arg_1, Variant arg_2, Variant arg_3) { return func(arg_0, arg_1, arg_2, arg_3); }
	std::function<Variant(Variant&, Variant&, Variant&, Variant&)> func;
};