#pragma once

#include "Vector.h"

#include "core/titanscript/ScriptNode.h"
#include "core/titanscript/TsVariable.h"
#include "Globals.h"
#include "TMessage.h"
#include "CoreNames.h"
#include "Memory.h"
#include "core/String.h"
#include "Map.h"

class Line;
class Function;
class State;
class Class;

struct Token
{
	Token(const String &txt, int t)
	{
		text = txt;
		type = t;
	}
	String text;
	int type = 0;

	enum Type
	{
		UNDEF,
		OPERATOR,
		KEYWORD, 
		WORD, 
		NUMBER,
		STRING, 
		TAB
	};
};

class Line
{
public:
	Line() { };
	Line(Array<Token> t) { tokens = t; }
	~Line() {  };

	void Free()
	{
		/*
		for (int c = 0; c < sub.size(); c++)
		{
			sub[c]->Free();
			delete sub[c];
		}
		sub.clear();
		*/
	}

	int size() const { return tokens.size(); }
	bool StartsWith(const String &txt) const { return tokens[0].text == txt; }
	bool StartsWith(const int &type) const { return tokens[0].type == type; }
	bool EndsWith(const String &txt) const { return tokens[size() - 1].text == txt; }
	bool EndsWith(const int &type) const { return tokens[size() - 1].type == type; }
	bool Contains(const String &txt) const { return Search(txt) != -1; }
	bool ContainsOutside(const String &elm) const
	{
		int level = 0;				
		for (int c = 0; c < tokens.size(); c++)
		{
			if (tokens[c].text == "(" || tokens[c].text == "[") level++;
			if (tokens[c].text == ")" || tokens[c].text == "]") level--;
			if (tokens[c].text == elm && level == 0) return true;
		}
		return false;
	}
	int SearchOutside(const String &elm) const
	{
		int level = 0, c;
		for (c = 0; c < tokens.size(); c++)
		{
			if (tokens[c].text == "(" || tokens[c].text == "[") level++;
			if (tokens[c].text == ")" || tokens[c].text == "]") level--;
			if (tokens[c].text == elm && level == 0) return c;
		}
		return c;
	}
	int Count(const String &text) const
	{
		int level = 0, count = 0;
		for (int c = 0; c < tokens.size(); c++)
		{
			if (tokens[c].text == "(" || tokens[c].text == "[") level++;
			if (tokens[c].text == ")" || tokens[c].text == "]") level--;
			if (tokens[c].text == text && level == 0) count++;
		}
		return count;
	}
	Array<int> GetIndices(const String &text) const
	{
		int level = 0;
		Array<int> indices;
		for (int c = 0; c < tokens.size(); c++)
		{
			if (tokens[c].text == "(" || tokens[c].text == "[") level++;
			if (tokens[c].text == ")" || tokens[c].text == "]") level--;
			if (tokens[c].text == text && level == 0) indices.push_back(c);
		}
		return indices;
	}

	int Search(const String &txt) const
	{
		for (int c = 0; c < tokens.size(); c++)
			if (tokens[c].text == txt)
				return c;

		return -1;
	}

	int search_last(const String &txt) const
	{
		for (int c = tokens.size() - 1; c >= 0; c--)
			if (tokens[c].text == txt)
				return c;

		return -1;
	}
	
	Array<String> strings;
	Array<Token> tokens;
	Vector<Line> sub;
	ScriptNode *node;

	int level = 0;
};

class Function
{
public:
	Function(String na, Block *no)
	{
		name = na;
		block = no;
	}

	String name;
	Block *block;
};

class State
{
public:
	State() {}

	void Free()
	{
		for (std::pair<String, TsVariable*> v : vars)
			delete v.second;

		for (std::pair<String, Function*> f : funcs)
			delete f.second;

		vars.clear();
		funcs.clear();
		poppara.clear();
	}

	bool VarExists(StringName name) { return vars.count(name) > 0; }
	void AddVar(StringName name) { vars.set(name, new TsVariable(name)); }
	bool FuncExists(StringName name) { return funcs.count(name) > 0; }
	void AddFunc(Function *func) { funcs.set(func->name, func); }

	void SetVar(const StringName& name, const Variant &val)
	{
		if (!VarExists(name))
			AddVar(name);
		else
		{
			DeleteVar(name);
			AddVar(name);
		}
		vars[name]->value = val;
	}
	Variant* GetVar(const StringName &name)
	{
		if (VarExists(name))
			return &vars[name.get_source()]->value;
		
		T_ERROR("Var: " + name.operator std::string() + " does not exist!");
		return nullptr;
	}
	TsVariable* DeleteVar(const StringName &name)
	{
		if (VarExists(name))
		{
			GC->queue_clean(vars[name]->value);
			vars.clear(name);
		}
		else
			T_ERROR("Var: " + name.operator std::string() + " does not exist!");

		return 0;
	}
	Function* GetFunc(const StringName &name)
	{
		if (FuncExists(name))
			return funcs[name];
		else
			T_ERROR("Func: " + name.operator std::string() + " does not exist!");

		return 0;
	}
	Array<Variant> GetReturns()
	{
		Array<Variant> result = returnstack;
		returnstack.clear();
		return result;
	}
	Array<Variant>& GetArgs()
	{
		return arg_stack;
	}

	//Get parameters
	Variant getval(int i) { return arg_stack[i]; }

	//Set parameters
	void setpara(int i, const Variant &newv) { arg_stack.set(0, newv); }

	//Check parameters
	bool isnumber(int i) { return arg_stack[i].type == CORE_TYPE(Real); }
	bool isbool(int i) { return arg_stack[i].type == CORE_TYPE(Bool); }
	bool isstring(int i) { return arg_stack[i].type == CORE_TYPE(String); }

	//Add
	void addreturn(const Variant &v) { returnstack.push_back(v); }
	void addparam(const Variant &v) { arg_stack.push_back(v); }

	//Clear
	void clearreturns() { returnstack.clear(); }
	void clearparams() { arg_stack.clear(); }

	//Pop and push
	void popparas() { poppara = arg_stack; }
	void pushparas() { arg_stack = poppara; }
	void popreturns() { popreturn = returnstack; }
	void pushreturns() { returnstack = popreturn; }

	//Sizes
	int argcount() { return arg_stack.size(); }
	int returncount() { return returnstack.size(); }

	//Extends
	Variant extension;
	VariantType extensiontype; //extensions can only inherit Object

private:
	Array<Variant> arg_stack, returnstack, poppara, popreturn;
	Map<String, TsVariable> vars;
	Map<String, Function> funcs;
};