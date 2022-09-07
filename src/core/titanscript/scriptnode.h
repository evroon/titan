#pragma once

#include "core/variant/variant.h"
#include "types/method.h"
#include "core/property.h"

struct Init;

struct ScriptNode
{
public:
	ScriptNode();
	virtual ~ScriptNode() {}

	virtual void clean() { }

	enum Type
	{
		UNDEF, CONSTANT, EXTENDS,
		INIT, ARRAY_INIT, VALUE, SUM,
		PRODUCT, PARENTHESES, IF,
		IFELEMENT, AND, OR,
		VARIABLE, ARRAY_INDEXING, FUNCTIONINIT,
		BLOCK, FUNCTIONCALL, RETURN, WHILE,
		FOR, COMPOSITION, COMPARISON,
		CHANGEONE, MODIFY, PATH,
		PATHORIGIN, ORIENTATION, NOT,
		STATICFUNC, STATICVAR, SUPERVAR,
		SUPERFUNC, MEMBERFUNC, MEMBERVAR,
		CONSTRUCTOR, TYPE_SPECIFIER
	};

	Type type = UNDEF;

	bool isconst = false;

	int GetType() { return type; }
};

struct Constant : ScriptNode
{
	Constant() { type = CONSTANT; }
	Constant(const Variant &p_value) : Constant() { value = p_value; }

	Variant value;
};

struct Extends : ScriptNode
{
	Extends() { type = EXTENDS; }
	Extends(const StringName &n) { exttype = n; type = EXTENDS; }

	StringName exttype;
};

struct Init : ScriptNode
{
	Init() { type = INIT; }
	Init(ScriptNode *vl, ScriptNode *vr) { val = vl; var = vr; type = INIT; }
	ScriptNode *val, *var;
};
struct ArrayInit : ScriptNode
{
	ArrayInit() { type = ARRAY_INIT; }
	ArrayInit(const Vector<ScriptNode> &p_nodes) { nodes = p_nodes; type = ARRAY_INIT; }

	Vector<ScriptNode> nodes;
};
struct Sum : ScriptNode
{
	Sum() { type = SUM; }
	Sum(ScriptNode *l, ScriptNode *r, const StringName &s) : Sum() { op = s; left = l; right = r; }
	ScriptNode *left, *right;
	StringName op;
};
struct Product : ScriptNode
{
	Product() { type = PRODUCT; }
	Product(ScriptNode *l, ScriptNode *r, const StringName &s) { op = s; left = l; right = r; type = PRODUCT; }
	ScriptNode *left, *right;
	StringName op;
};
struct Parentheses : ScriptNode
{
	Parentheses() { type = PARENTHESES; }
	Parentheses(ScriptNode *n) { node = n; type = PARENTHESES; }
	ScriptNode *node;
};
struct ArrayIndexing : ScriptNode
{
	ArrayIndexing() { type = ARRAY_INDEXING; }
	ArrayIndexing(ScriptNode *p_array, ScriptNode *p_index) { array = p_array; index = p_index; type = ARRAY_INDEXING; }

	ScriptNode *array;
	ScriptNode *index;
};
struct Block : ScriptNode
{
	Block() { type = BLOCK; }
	Block(Vector<ScriptNode> ls) { lines = ls; type = BLOCK; }
	Block(Vector<ScriptNode> ls, Vector<ScriptNode> ps) { params = ps; lines = ls; type = BLOCK; }

	Vector<ScriptNode> params, lines;
	bool isfunction = false;
};
struct FunctionInit : ScriptNode
{
	FunctionInit() { type = FUNCTIONINIT; }
	FunctionInit(Block *n, const StringName &s) { name = s; block = n; type = FUNCTIONINIT; }

	Block *block;
	StringName name;
};
struct FunctionCall : ScriptNode
{
	FunctionCall() { type = FUNCTIONCALL; }
	FunctionCall(const StringName &n, Vector<ScriptNode> ps) { params = ps; name = n; type = FUNCTIONCALL; }

	StringName name;
	Vector<ScriptNode> params;
};
struct IfElement : ScriptNode
{
	IfElement() { type = IFELEMENT; }
	IfElement(ScriptNode *p, Block *b) { node = b; passtest = p; type = IFELEMENT; }
	ScriptNode *passtest;
	Block *node;
	StringName name;
};
struct If : ScriptNode
{
	If() { type = IF; }
	If(Vector<IfElement> e) { elements = e; type = IF; }
	Vector<IfElement> elements;
};
struct And : ScriptNode
{
	And() { type = AND; }
	And(ScriptNode *l, ScriptNode *r) { left = l; right = r; type = AND; }
	ScriptNode *left, *right;
};
struct Or : ScriptNode
{
	Or() { type = OR; }
	Or(ScriptNode *l, ScriptNode *r) { left = l; right = r; type = OR; }
	ScriptNode *left, *right;
};
struct Return : ScriptNode
{
	Return() { type = RETURN; }
	Return(ScriptNode *v) { val = v; type = RETURN; }

	ScriptNode *val;
};
struct ForLoop : ScriptNode
{
	ForLoop() { type = FOR; }
	ForLoop(ScriptNode *p, ScriptNode *u, ScriptNode *d, ScriptNode *f) { passcheck = p; update = u; decl = d; func = f; type = FOR; }

	ScriptNode *passcheck, *update, *decl, *func;
};
struct WhileLoop : ScriptNode
{
	WhileLoop() { type = WHILE; }
	WhileLoop(ScriptNode *p, ScriptNode *f) { func = f; passcheck = p; type = WHILE; }

	ScriptNode *passcheck, *func;
};
struct Composition : ScriptNode
{
	Composition() { type = COMPOSITION; }
	Composition(Vector<ScriptNode> n) { nodes = n; type = COMPOSITION; }

	Vector<ScriptNode> nodes;
};
struct Comparison : ScriptNode
{
	Comparison() { type = COMPARISON; }
	Comparison(const StringName &n, ScriptNode *l, ScriptNode *r) { name = n; left = l; right = r; type = COMPARISON; }

	ScriptNode *left, *right;
	StringName name;
};
struct ChangeOne : ScriptNode
{
	ChangeOne() { type = CHANGEONE; }
	ChangeOne(ScriptNode *v, const StringName &o) { var = v; op = o; type = CHANGEONE; }

	ScriptNode *var;
	StringName op;
};
struct Modify : ScriptNode
{
	Modify() { type = MODIFY; }
	Modify(ScriptNode *vl, ScriptNode *vr, const StringName &o) { val = vl; var = vr; op = o; type = MODIFY; }

	ScriptNode *val, *var;
	StringName op;
};
struct PathOrigin : ScriptNode
{
	PathOrigin() { type = PATHORIGIN; }
	ScriptNode *node;
};
struct Path : ScriptNode
{
	Path() { type = PATH; }
	Path(Vector<ScriptNode> p, const StringName &n) { path = p; type = PATH; }

	PathOrigin *origin;
	Vector<ScriptNode> path;
};
struct MemberFunc : ScriptNode
{
	MemberFunc() { type = MEMBERFUNC; }
	Method *bounded_method = NULL;
	StringName method_name;
	Vector<ScriptNode> args;
};
struct MemberVar : ScriptNode
{
	MemberVar() { type = MEMBERVAR; }

	StringName variable_name;
	Property *getset;
};
struct VariableNode : ScriptNode
{
	VariableNode() { type = VARIABLE; }
	VariableNode(const StringName &n) { name = n; type = VARIABLE; }

	StringName name;
};
struct Orientation : ScriptNode
{
	Orientation() { type = ORIENTATION; }
	Orientation(ScriptNode *r) { right = r; type = ORIENTATION; }

	ScriptNode *right;
	char o;
};
struct Not : ScriptNode
{
	Not() { type = NOT; }
	Not(ScriptNode *r) { right = r; type = NOT; }

	ScriptNode *right;
};
struct StaticFuncCall : ScriptNode
{
	StaticFuncCall() { type = STATICFUNC; }
	StaticFuncCall(const StringName &n, Vector<ScriptNode> ps) { params = ps; name = n; type = STATICFUNC; }

	StringName name;
	Vector<ScriptNode> params;
};
struct StaticVariable : ScriptNode
{
	StaticVariable() { type = STATICVAR; }
	StaticVariable(const StringName &n) { name = n; type = STATICVAR; }

	StringName name;
};
struct SuperVariable : ScriptNode
{
	SuperVariable() { type = SUPERVAR; }

	Property *property;
};
struct SuperFunction : ScriptNode
{
	SuperFunction() { type = SUPERFUNC; }
	SuperFunction(const StringName &n, Vector<ScriptNode> ps) { params = ps; name = n; type = SUPERFUNC; }

	StringName name;
	Vector<ScriptNode> params;
};
struct Constructor : ScriptNode
{
	Constructor() { type = CONSTRUCTOR; }
	Constructor(const StringName &n, Vector<ScriptNode> ps) { params = ps; name = n; type = CONSTRUCTOR; }

	StringName name;
	Vector<ScriptNode> params;
};
struct TypeSpecifier : ScriptNode
{
	TypeSpecifier() { type = TYPE_SPECIFIER; }
	TypeSpecifier(const VariantType &p_referenced_type) { referenced_type = p_referenced_type; type = TYPE_SPECIFIER; }

	VariantType referenced_type;
};