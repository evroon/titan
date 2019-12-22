#include "Parser.h"

#include "core/Array.h"
#include "types/MethodMaster.h"
#include "Executer.h"

Parser::Parser(State *_state, Line &root)
{
	warnings = Array<ParseWarning>();
	errors = Array<ParseError>();
	definitions = Array<Definition>();
	parent_type = VariantType();
	subindex = 0;

	state = _state;
	parent = &root;
	Parse(root);
}

void Parser::Parse(Line &root)
{
	//StaticFunctions::Init();
	for (int c = 0; c < root.sub.size(); c++)
		root.sub[c]->node = ParsePart(*root.sub[c]);
}

int Parser::GetFirstIndex(const Array<Token> &tokens, const String src[], int srccount)
{
	int ind = -1, level = 0;
	for (int c = 0; c < tokens.size(); c++)
	{
		if (tokens[c].text == "(") level++;
		if (tokens[c].text == ")") level--;

		for (int i = 0; i < srccount; i++)
			if ((tokens[c].text == src[i]) && level == 0)
			{ 
				ind = c;
				break;
			}
	}
	return ind;
}

Composition* Parser::GetComposition(const Line &line)
{
	if (line.tokens.size() == 0)
		return new Composition;

	Composition *comp = new Composition;
	Array<Token> buf;
	Array<Array<Token>> bufs;
	int level = 0;

	for (int c = 0; c < line.tokens.size(); c++)
	{
		if (line.tokens[c].text == "(" || line.tokens[c].text == "[") level++;
		if (line.tokens[c].text == ")" || line.tokens[c].text == "]") level--;

		if (line.tokens[c].text != "," || level > 0)
			buf.push_back(line.tokens[c]);
		else
		{
			bufs.push_back(buf);
			buf.clear();
		}
	}

	bufs.push_back(buf);
	buf.clear();

	for (int c = 0; c < bufs.size(); c++)
		comp->nodes.push_back(ParsePart(bufs[c]));

	return comp;
}

Block* Parser::ParseBlock(const Line &line)
{
	Line l = line;
	Vector<ScriptNode> nodes = Vector<ScriptNode>();
	int count = l.sub.size();

	if (count == 0)
		PARSE_WARNING("Expected a block");

	parent = const_cast<Line*>(&line);

	for (int c = 0; c < count; c++)
	{
		subindex = c;
		nodes.push_back(ParsePart(*l.sub[c]));
	}

	return new Block(nodes);
}

ScriptNode* Parser::ParsePart(const Line &line)
{
	if (line.tokens.size() < 1 || line.StartsWith("else"))
		return nullptr;
	else if (line.StartsWith("elseif"))
		return nullptr;
	else if (line.StartsWith("define"))
	{
		ScriptNode *node = ParsePart(line.tokens.getrest(2));

		StringName name = line.tokens[1].text;
		Variant value = SimpleExecuter::execute(node);

		definitions.push_back({ name, value });
		return nullptr;
	}
	else if (line.StartsWith("extends"))															//Inheritance
	{
		Extends *e = new Extends(line.tokens[1].text);
		state->extensiontype = GETTYPE(StringName(line.tokens[1].text));
		return e;
	}
	else if (line.StartsWith("while"))																//While Loop
	{
		WhileLoop *loop = new WhileLoop;
		loop->passcheck = ParsePart(line.tokens.getrest(1));
		loop->func = ParseBlock(line);
		return loop;
	}
	else if (line.StartsWith("for"))																//For Loop
	{
		ForLoop *loop = new ForLoop;
		loop->func = ParseBlock(line);
		Line l = Line(line.tokens.getrest(1));
		Composition *comp = GetComposition(l);

		if (comp->nodes.size() != 3)
			PARSE_ERROR("Expected 3 nodes to construct for-loop");

		for (int c = 0; c < comp->nodes.size(); c++)
			if (comp->nodes[c]->type == ScriptNode::INIT)
				loop->decl = comp->nodes[c];
			else if (comp->nodes[c]->type == ScriptNode::COMPARISON)
				loop->passcheck = comp->nodes[c];
			else
				loop->update = comp->nodes[c];	//Expect update statement

			return loop;
	}
	else if (line.StartsWith("{") && line.EndsWith("}"))											//Array Init
	{
		ArrayInit *ai = new ArrayInit;

		Line l = line.tokens.split(1, line.tokens.size() - 2);

		Composition *comp = GetComposition(l);
		
		ai->nodes = comp->nodes;
		return ai;
	}
	else if (line.Contains("="))																	//Init
	{
		Init *init = new Init;
		init->val = ParsePart(line.tokens.getrest(line.Search("=") + 1));
		init->var = ParsePart(line.tokens.split(0, line.Search("=") - 1));

		if (init->var->type != ScriptNode::SUPERVAR && init->var->type != ScriptNode::VARIABLE && init->var->type != ScriptNode::PATH)
			PARSE_ERROR("Expected a variable to initialize");

		return init;
	}
	else if (line.StartsWith("func"))																//Init Func
	{
		FunctionInit *init = new FunctionInit;
		Block* node = new Block;

		if (line.tokens.size() < 3)
			PARSE_ERROR("Expected function definition");

		if (line.tokens[2].text != "(" || !line.Contains(")"))
			PARSE_ERROR("Function not well-defined");

		Line l = Line(line.tokens.split(3, line.tokens.size() - 2));

		Composition *comp = GetComposition(l);

		for (ScriptNode *n : comp->nodes)
			node->params.push_back(n);

		init->name = StringName(line.tokens[1].text);
		init->block = ParseBlock(line);
		init->block->params = node->params;
		init->block->isfunction = true;

		return init;
	}
	else if (line.StartsWith("if"))																	//If
	{
		If *ifstat = new If;
		IfElement *e;
		Line l = line, par = *parent;
		int index = subindex;

		while ((l.StartsWith("if") && ifstat->elements.size() == 0) || l.StartsWith("elseif") || l.StartsWith("else")) //Line must begin with if, elseif, else
		{
			e = new IfElement;
			e->name = StringName(l.tokens[0].text);
			if (!l.StartsWith("else"))
				e->passtest = ParsePart(l.tokens.getrest(1));
			e->node = ParseBlock(l);
			ifstat->elements.push_back(e);

			if (++index < par.sub.size())
				l = *par.sub[index];
			else break;
		}
		return ifstat;
	}
	else if (line.StartsWith("return"))																//Return
	{
		Return *re = new Return;
		if (line.tokens.size() > 1)
			re->val = ParsePart(line.tokens.getrest(1));
		return re;
	}
	else if (line.ContainsOutside("+=") || line.ContainsOutside("-=") ||
		line.ContainsOutside("*=") || line.ContainsOutside("/="))									//Modify
	{
		Modify *mod = new Modify;

		if (line.ContainsOutside("+="))			mod->op = String("+=");
		else if (line.ContainsOutside("-="))	mod->op = "-=";
		else if (line.ContainsOutside("*="))	mod->op = "*=";
		else									mod->op = "/=";

		mod->val = ParsePart(line.tokens.getrest(line.Search(mod->op) + 1));
		mod->var = ParsePart(line.tokens.split(0, line.Search(mod->op) - 1));
		return mod;
	}
	else if (line.ContainsOutside(","))																//Composition
	{
		return GetComposition(line);
	}
	else if (line.StartsWith("-") || line.StartsWith("+"))											//Orientation
	{
		Orientation *o = new Orientation;
		Array<Token> ts;
		ts.push_back(line.tokens.split(1, line.tokens.size() - 1));
		o->right = ParsePart(ts);
		o->o = line.tokens[0].text[0];

		return o;
	}
	else if (line.ContainsOutside("+") || line.ContainsOutside("-"))								//Sum
	{
		Sum *sum = new Sum;
		String arr[] = { "+", "-" };
		int ind = GetFirstIndex(line.tokens, arr, 2);

		sum->op = line.tokens[ind].text;

		if (line.Search(sum->op) > 0)
			sum->left = ParsePart(line.tokens.split(0, line.Search(sum->op) - 1));

		sum->right = ParsePart(line.tokens.getrest(line.Search(sum->op) + 1));
		return sum;
	}
	else if (line.ContainsOutside("*") || line.ContainsOutside("/"))								//Product
	{
		Product *pro = new Product;
		String arr[] = { "*", "/" };
		int ind = GetFirstIndex(line.tokens, arr, 2);

		pro->op = String(line.tokens[ind].text);

		if (line.Search(pro->op) > 0)
			pro->left = ParsePart(line.tokens.split(0, line.Search(pro->op) - 1));

		pro->right = ParsePart(line.tokens.getrest(line.Search(pro->op) + 1));
		return pro;
	}
	else if (line.ContainsOutside("&&"))															//And
	{
		And *a = new And;
		a->left = ParsePart(line.tokens.split(0, line.Search("&&") - 1));
		a->right = ParsePart(line.tokens.getrest(line.Search("&&") + 1));
		return a;
	}
	else if (line.ContainsOutside("||"))															//Or
	{
		Or * o = new Or;
		o->left = ParsePart(line.tokens.split(0, line.Search("||") - 1));
		o->right = ParsePart(line.tokens.getrest(line.Search("||") + 1));
		return o;
	}
	else if (line.ContainsOutside("<=") || line.ContainsOutside(">=") ||
		line.ContainsOutside("==") || line.ContainsOutside("!=") ||
		line.ContainsOutside(">") || line.ContainsOutside("<"))										//Comparison
	{
		Comparison *comp = new Comparison;
		const String arr[] = { "<=", ">=", "==", "!=", ">", "<" };
		int ind = GetFirstIndex(line.tokens, arr, 6);
		comp->name = line.tokens[ind].text;
		comp->left = ParsePart(line.tokens.split(0, line.Search(comp->name) - 1));
		comp->right = ParsePart(line.tokens.getrest(line.Search(comp->name) + 1));
		return comp;
	}
	else if (line.StartsWith("("))																	//Parentheses
	{
		Parentheses *par = new Parentheses;
		int ind = line.search_last(")");
		par->node = ParsePart(line.tokens.split(1, ind - 1));
		return par;
	}
	/*else if (line.StartsWith("[") && line.Contains("]"))											//Index specifier
	{
	Variable *ind = new Variable();
	ind->key = ParsePart(line.tokens.split(1, line.Search("]") - 1));
	return ind;
	}*/
	else if (line.StartsWith(Token::NUMBER) && line.size() == 1)									//Number
	{
		Variant val = Variant(new Real(line.tokens[0].text));
		return new Constant(val);
	}
	else if ((line.StartsWith("true") || line.StartsWith("false")) && line.size() == 1)				//bool keyword
	{
		Variant val = (bool)String(line.tokens[0].text);
		return new Constant(val);
	}
	else if (line.StartsWith(Token::WORD) && line.size() > 1 && line.tokens[1].text == "(" &&  line.SearchOutside(")") == line.tokens.size() - 1)			//Function Call
	{
		String name = line.tokens[0].text;
		StringName sname = name;

		//The arguments
		Line l = Line(line.tokens.split(2, line.tokens.size() - 2));
		Composition *comp = GetComposition(l);

		if (MMASTER->static_funcs.contains(sname))													//Static Function
		{
			StaticFuncCall *sfc = new StaticFuncCall;
			sfc->name = sname;

			for (ScriptNode *n : comp->nodes)
				sfc->params.push_back(n);

			return sfc;
		}
		else if (TypeManager::get_singleton()->type_exists(sname))
		{
			Constructor *cstr = new Constructor;
			cstr->name = sname;

			for (ScriptNode *n : comp->nodes)
				cstr->params.push_back(n);

			return cstr;
		}
		else if (MMASTER->method_exists(VariantType(state->extensiontype), sname))
		{
			SuperFunction *sf = new SuperFunction;
			sf->name = sname;

			for (ScriptNode *n : comp->nodes)
				sf->params.push_back(n);
			
			return sf;
		}
		else
		{
			FunctionCall *call = new FunctionCall;
			call->name = sname;

			for (ScriptNode *n : comp->nodes)
				call->params.push_back(n);

			return call;
		}		
	}
	else if (line.EndsWith("++") || line.EndsWith("--"))											//Add or subtract one
	{
		ChangeOne *one = new ChangeOne;
		one->op = line.tokens[line.tokens.size() - 1].text;

		Array<Token> ts = line.tokens.split(0, line.tokens.size() - 2);
		one->var = ParsePart(ts);
		return one;
	}
	else if (line.EndsWith("]"))																	//Array Indexing
	{
		ArrayIndexing *ai = new ArrayIndexing;

		Line array = line.tokens.split(0, line.Search("[") - 1);
		Line index = line.tokens.split(line.Search("[") + 1, line.tokens.size() - 2);

		ai->array = ParsePart(array);
		ai->index = ParsePart(index);

		return ai;
	}
	else if (line.StartsWith(Token::WORD))															//Variable
	{
		//Concatenate strings to create name
		String name;
		for (int c = 0; c < line.tokens.size(); c++)
			name += line.tokens[c].text;

		//Is it a static variable?
		Definition *def = get_definition(name);
		if (def)
			return new Constant(def->value);

		//Is it a member variable? 
		else if (!line.ContainsOutside(".") && MethodMaster::get_method_master()->property_exists(VariantType(state->extensiontype), StringName(line.tokens[0].text)))
		{
			SuperVariable *super_var = new SuperVariable;
			super_var->property = MethodMaster::get_method_master()->get_property(
				VariantType(state->extensiontype), StringName(line.tokens[0].text));
			return super_var;
		}

		//Is it a simple single variable or a type specifier?
		if (!line.ContainsOutside("."))
		{
			if (TYPEMAN->type_exists(line.tokens[0].text))
				return new TypeSpecifier(TYPEMAN->get_type(line.tokens[0].text));
			else
				return new VariableNode(line.tokens[0].text);
		}

		//It is more complicated		
		return ParsePath(line);
	}
	else if (line.StartsWith("!"))																	//Not
	{
		Not *n = new Not;
		Array<Token> ts;
		ts.push_back(line.tokens[1]);
		n->right = ParsePart(ts);

		return n;
	}
	else if (line.tokens[0].text[0] == '"' && line.tokens[0].text[line.tokens[0].text.length() - 1] == '"')			//String
	{
		String txt(line.tokens[0].text.substr(1, line.tokens[0].text.length() - 2));
		Variant var = new String(txt);
		Constant *v = new Constant(var);
		return v;
	}
	PARSE_ERROR("Unrecognized statement found while parsing line: " + line.tokens[0].text);
	return 0;
}

ScriptNode *Parser::ParsePath(const Line &line)
{
	//Get indices in line of dots
	Array<int> indices = line.GetIndices(".");
	indices.push_back(line.tokens.size());

	Path *var = new Path;
	var->origin = new PathOrigin;
	var->origin->node = ParsePart(Line(line.tokens.split(0, indices[0] - 1)));


	for (int c = 0; c < indices.size() - 1; c++)
	{
		int start = indices[c] + 1, end = indices[c + 1] - 1;
		ScriptNode *n = ParseMember(line.tokens.split(start, end));
		var->path.push_back(n);
	}
	return var;
}

ScriptNode *Parser::ParseMember(const Line &line)
{
	if (line.StartsWith(Token::WORD) && line.size() > 1 && line.tokens[1].text == "(")
		return ParseMemberFunc(line);
	else if(line.StartsWith(Token::WORD) && line.size() == 1)
		return ParseMemberVar(line);

	return nullptr;
}

ScriptNode *Parser::ParseMemberFunc(const Line &line)
{
	MemberFunc *call = new MemberFunc;
	String name = line.tokens[0].text;
	Line l = Line(line.tokens.split(2, line.tokens.size() - 2));
	Composition *comp = GetComposition(l);

	for (int c = 0; c < comp->nodes.size(); c++)
		call->args.push_back(comp->nodes[c]);

	//call->bounded_method = MethodMaster::get_method_master()->get_method(parent_type, StringName(name));
	call->method_name = name;

	if (call->bounded_method && call->bounded_method->returns_variant)
	{
		ReturnMethod *r_m = reinterpret_cast<ReturnMethod*>(call->bounded_method);
		parent_type = r_m->return_type;
	}

	return call;
}

ScriptNode *Parser::ParseMemberVar(const Line &line)
{
	MemberVar *mem_var = new MemberVar;
	String name = line.tokens[0].text;
	
	mem_var->variable_name = name;

	return mem_var;
}

void Parser::report_error(const ParseError &err)
{
	errors.push_back(err);
}

void Parser::report_warning(const ParseWarning &warn)
{
	warnings.push_back(warn);
}

Parser::Definition* Parser::get_definition(const StringName &p_name)
{
	for (Parser::Definition &def : definitions)
		if (def.name == p_name)
			return &def;

	return nullptr;
}
