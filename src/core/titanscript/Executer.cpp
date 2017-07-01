#include "Executer.h"

#include "core/Memory.h"
#include "types/MethodMaster.h"

Executer::Executer()
{

}

Executer::Executer(Line line, State *state)
{
	this->state = state;

	for (int c = 0; c < line.sub.size(); c++)
		Execute(line.sub[c]->node);
}

Variant Executer::run_method(Method *m, Array<Variant> &args)
{
	if (m->returns_variant)
	{
		switch (m->arg_count)
		{
		case 0:
			return dynamic_cast<R_Method_0*>(m)->operator()();

		case 1:
			return dynamic_cast<R_Method_1*>(m)->operator()(
				args[0]);

		case 2:
			return dynamic_cast<R_Method_2*>(m)->operator()(
				args[0], args[1]);

		case 3:
			return dynamic_cast<R_Method_3*>(m)->operator()(
				args[0], args[1], args[2]);

		case 4:
			return dynamic_cast<R_Method_4*>(m)->operator()(
				args[0], args[1], args[2], args[3]);
		}
	}
	else
	{
		switch (m->arg_count)
		{
		case 0:
			dynamic_cast<V_Method_0*>(m)->operator()();
			break;

		case 1:
			dynamic_cast<V_Method_1*>(m)->operator()(
				args[0]);
			break;

		case 2:
			dynamic_cast<V_Method_2*>(m)->operator()(
				args[0], args[1]);
			break;

		case 3:
			dynamic_cast<V_Method_3*>(m)->operator()(
				args[0], args[1], args[2]);
			break;

		case 4:
			dynamic_cast<V_Method_4*>(m)->operator()(
				args[0], args[1], args[2], args[3]);
			break;
		}
		return Variant();
	}
	return Variant();
}

Variant Executer::run_member_func(Variant &object, MemberFunc *mf)
{
	Array<Variant> args;
	args.push_back(object);

	for (int c = 0; c < mf->args.size(); c++)	//Get arguments
		args.push_back(Execute(mf->args[c]));

	VariantType t = object.get_type();

	Method *m = MMASTER->get_method(t, mf->method_name);

	if (!m)
	{
		T_ERROR("Could not find method: " + mf->method_name);
		return Variant();
	}

	return run_method(m, args);
}

Variant Executer::GetMemberMinusOne(const Path &var)
{
	Path p = var;
	p.path.removelast();
	return GetMember(p);
}

Variant Executer::GetMember(const Path &var)
{
	Variant cur = Execute(var.origin->node);
	
	for (int c = 0; c < var.path.size(); c++)		//Get each member
	{
		ScriptNode *n = var.path[c];

		if (!cur.isdef())
		{
			T_ERROR("Path error");
			return NULL_VAR;
		}

		if (n->type == ScriptNode::MEMBERVAR)					//Get member variable
		{
			MemberVar *memvar = dynamic_cast<MemberVar*>(n);

			Property *p = MMASTER->get_property(cur.get_type(), memvar->variable_name);

			if (p)
				cur = p->get->operator()(cur);
			else
			{
				T_ERROR("Path error");
				return NULL_VAR;
			}
		}

		else if (n->type == ScriptNode::MEMBERFUNC)			//Execute member function
		{
			MemberFunc *fc = (MemberFunc*)n;
			cur = run_member_func(cur, fc);
		}
	}
		
	return cur; 
}

Property* Executer::get_property(const Path &var)
{
	Variant cur = Execute(var.origin->node);
	MemberVar *memvar = NULL;
	Property *p = NULL;

	for (int c = 0; c < var.path.size(); c++)		//Get each member
	{
		ScriptNode *n = var.path[c];

		if (n->type == ScriptNode::MEMBERVAR)					//Get member variable
		{
			memvar = dynamic_cast<MemberVar*>(n);

			p = MMASTER->get_property(cur.get_type(), memvar->variable_name);

			if (p)
				cur = p->get->operator()(cur);
			else
			{
				T_ERROR("Property does not exist");
				cur = Variant();
			}
		}

		else if (n->type == ScriptNode::MEMBERFUNC)			//Execute member function
		{
			MemberFunc *fc = (MemberFunc*)n;
			cur = run_member_func(cur, fc);
		}
	}

	return p;
}

void Executer::SetVariable(ScriptNode *node, Variant val)
{
	if (node->GetType() == ScriptNode::PATH)
	{
		Path path = *reinterpret_cast<Path*>(node);

		ScriptNode *last = path.path[path.path.size() - 1];
		Property *p = get_property(path);

		if (last->type != ScriptNode::MEMBERVAR)
			T_ERROR("Can only assign a value to a variable");
		else
		{
			Variant v = GetMemberMinusOne(path);

			if (p)
			{
				p->set->operator()(v, val);
				GC->queue_clean(val);
			}
			else
				T_ERROR("Property does not exist");
		}
	}
	else if (node->GetType() == ScriptNode::VARIABLE)
	{
		VariableNode *var = (VariableNode*)node;
		state->SetVar(var->name, val);
	}
	else if (node->GetType() == ScriptNode::SUPERVAR)
	{
		SuperVariable *var = (SuperVariable*)node;
		var->property->set->operator()(state->extension, val);
		GC->queue_clean(val);
	}
}

Variant Executer::run_titan_func(const String &name, Array<Variant> paras)
{
	state->popparas();
	state->clearparams();

	for (int c = 0; c < paras.size(); c++)
		state->addparam(paras[c]);						//Add parameters to stack
	
	if (state->FuncExists(StringName(name)))
		Execute(state->GetFunc(StringName(name))->block);			//Execute user-defined function

	state->clearparams();
	return state->GetReturns();							//Get and clear returns
}

Variant Executer::Execute(ScriptNode *node)
{
	if (!node)											//Ignore else and elseif statments
		return 0;

	int type = node->GetType();

	if (type == ScriptNode::INIT)
	{
		Init *init = (Init*)node;
		Variant newvalue = Execute(init->val);
		SetVariable(init->var, newvalue);
		return newvalue;
	}
	else if (type == ScriptNode::ARRAY_INIT)
	{
		ArrayInit *init = reinterpret_cast<ArrayInit*>(node);

		Variant array = Array<Variant>();

		for (ScriptNode *n : init->nodes)
			array.push_back(Execute(n));

		return array;
	}
	else if (type == ScriptNode::ARRAY_INDEXING)
	{
		ArrayIndexing *array_indexing = reinterpret_cast<ArrayIndexing*>(node);
		Variant array = Execute(array_indexing->array);
		return array[Execute(array_indexing->index)];
	}
	else if (type == ScriptNode::SUM)
	{
		Sum *sum = (Sum*)node;
		Variant left = Execute(sum->left);
		Variant right = Execute(sum->right);
		Variant res = (sum->op == "+") ? left + right : left - right;

		GC->queue_clean(left);
		GC->queue_clean(right);

		return res;
	}
	else if (type == ScriptNode::PRODUCT)
	{
		Sum *sum = (Sum*)node;
		Variant left = Execute(sum->left);
		Variant right = Execute(sum->right);
		Variant res = (sum->op == "*") ? left * right : left / right;

		GC->queue_clean(left);
		GC->queue_clean(right);

		return res;
	}
	else if (type == ScriptNode::AND)
	{
		And *and = (And*)node;
		bool l = Execute(and->left);

		if (!l)
			return false;
		else
			return Execute(and->right);
	}
	else if (type == ScriptNode::OR)
	{
		Or *and = (Or*)node;
		bool l = Execute(and->left);

		if (l)
			return true;
		else
		{
			Variant right = Execute(and->right);
			if (right)
				return true;
		}
		return false;
	}
	else if (type == ScriptNode::CONSTANT)
	{
		Constant *constant = (Constant*)node;
		return constant->value.copy();
	}
	else if (type == ScriptNode::PARENTHESES)
	{
		Parentheses *val = (Parentheses*)node;
		return Execute(val->node);
	}
	else if (type == ScriptNode::PATH)
	{
		Path path = *reinterpret_cast<Path*>(node);
		return GetMember(path).copy();
	}
	else if (type == ScriptNode::VARIABLE)
	{
		VariableNode *var = (VariableNode*)node;
		
		Variant* value = state->GetVar(var->name);

		if (value)
			return *value;
		else
			return NULL_VAR;
	}
	else if (type == ScriptNode::SUPERVAR)
	{
		SuperVariable *var = (SuperVariable*)node;
		return var->property->get->operator()(state->extension);
	}
	else if (type == ScriptNode::IF)
	{
		If *ifstat = (If*)node;

		for (int c = 0; c < ifstat->elements.size(); c++)
		{
			IfElement *e = ifstat->elements[c];

			if (e->name == "if" || e->name == "elseif")
			{
				Variant res = Execute(e->passtest);

				if (res)
				{
					Execute(e->node);
					break;
				}
			}
			else
				Execute(e->node); //e->name == "else"
		}

		return 0;
	}
	else if (type == ScriptNode::BLOCK)
	{
		Block *block = (Block*) node;
		returntofunc = false;

		if (block->params.size() != state->argcount())
			T_ERROR("Number of arguments does not match");

		for (int c = 0; c < block->params.size(); c++) //Introduce parameters as local vars
		{
			VariableNode* var = (VariableNode*) block->params[c];
			state->SetVar(var->name, state->getval(c));
		}

		state->clearparams();

		for (int c = 0; c < block->lines.size(); c++) //Execute titancode
		{
			if ((!returntofunc && block->isfunction) || !block->isfunction)
				Execute(block->lines[c]);
			else if (returntofunc && !block->isfunction)
				break;
			else if (returntofunc && block->isfunction)
			{
				returntofunc = false;
				break;
			}
		}

		for (int c = 0; c < block->params.size(); c++) //Delete arguments
		{
			VariableNode* var = (VariableNode*)block->params[c];
			state->DeleteVar(var->name);
		}

		return 0;
	}
	else if (type == ScriptNode::FUNCTIONINIT)
	{
		FunctionInit *init = (FunctionInit*)node;
		state->AddFunc(new Function(init->name, init->block));

		return 0;
	}
	else if (type == ScriptNode::FUNCTIONCALL)
	{
		FunctionCall *call = (FunctionCall*)node;
		state->popparas();
		state->clearparams();

		for (int c = 0; c < call->params.size(); c++)
			state->addparam(Execute(call->params[c]));		//Add parameters to stack

		if (state->FuncExists(call->name.get_source()))
			Execute(state->GetFunc(call->name.get_source())->block);		//Execute user-defined function
		else if (call->name == StringName("print"))
			T_LOG(state->getval(0).ToString());
		else
			T_ERROR("Function: " + call->name.get_source() + " does not exist!");

		state->clearparams();
		return state->GetReturns();
	}
	else if (type == ScriptNode::STATICFUNC)
	{
		StaticFuncCall *call = (StaticFuncCall*)node;
		state->popparas();
		state->clearparams();

		for (int c = 0; c < call->params.size(); c++)
			state->addparam(Execute(call->params[c]));		//Add parameters to stack

		state->addreturn(run_method(MMASTER->static_funcs[call->name], Array<Variant>(state->getval(0))));

		state->pushparas();
		return state->GetReturns();
	}
	else if (type == ScriptNode::SUPERFUNC)
	{
		SuperFunction *call = (SuperFunction*)node;
		state->popparas();
		state->clearparams();

		state->addparam(state->extension);

		for (int c = 0; c < call->params.size(); c++)
			state->addparam(Execute(call->params[c]));		//Add parameters to stack
		
		Method *m = MMASTER->get_method(state->extensiontype, call->name);
		Variant result = run_method(m, state->GetArgs());

		state->pushparas();
		return result;
	}
	else if (type == ScriptNode::RETURN)
	{
		Return *re = (Return*)node;

		if (re->val)
		{
			const Variant &val = Execute(re->val);
			state->addreturn(val);
		}

		returntofunc = true;
		return 0;
	}
	else if (type == ScriptNode::FOR)
	{
		ForLoop *loop = (ForLoop*)node;
		Variant go;
		Execute(loop->decl);

		while (true)
		{
			go = Execute(loop->passcheck);
			if (!go) break;

			Execute(loop->func);
			Execute(loop->update);
		}

		//Delete counter
		Init* in = (Init*)loop->decl;
		Path* var = (Path*)in->var;
		//state->DeleteVar(var->name);
		return 0;
	}
	else if (type == ScriptNode::WHILE)
	{
		WhileLoop *loop = (WhileLoop*)node;
		Variant go;
		while (true)
		{
			go = Execute(loop->passcheck);
			if (!go) break;

			Execute(loop->func);
		}
		return 0;
	}
	else if (type == ScriptNode::CHANGEONE)
	{
		ChangeOne *one = (ChangeOne*)node;
		Variant var = Execute(one->var);
		Variant onenum = 1;

		SetVariable(one->var, var + onenum);
		onenum.clean();
		return 0;
	}
	else if (type == ScriptNode::COMPARISON)
	{
		Comparison *comp = (Comparison*)node;

		bool result = false;
		Variant left = Execute(comp->left);
		Variant right = Execute(comp->right);

		if		(comp->name == "<") result = left < right;
		else if (comp->name == ">") result = left > right;
		else if (comp->name == "<=") result = left <= right;
		else if (comp->name == ">=") result = left >= right;
		else if (comp->name == "==") result = left == right;
		else if (comp->name == "!=") result = left == right;

		return result;
	}
	else if (type == ScriptNode::COMPOSITION)
	{
		Composition *comp = (Composition*)node;
		Array<Variant> vals;

		for (int c = 0; c < comp->nodes.size(); c++)
			vals.push_back(Execute(comp->nodes[c]));

		return vals;
	}
	else if (type == ScriptNode::MODIFY)
	{
		Modify *mod = (Modify*)node;
		Variant val = Execute(mod->val);
		Variant var = Execute(mod->var);

		if		(mod->op == "+=") val = var + Execute(mod->val);
		else if (mod->op == "-=") val = var - Execute(mod->val);
		else if (mod->op == "*=") val = var * Execute(mod->val);
		else if (mod->op == "/=") val = var / Execute(mod->val);

		SetVariable(mod->var, val);
		return 0;
	}
	else if (type == ScriptNode::ORIENTATION)
	{
		Orientation *o = (Orientation*)node;
		Variant value = Execute(o->right);

		if (value.type == Variant::INT)
		{
			int mult = o->o == '+' ? 1 : -1;
			return Variant(Execute(o->right).i * mult);
		}
		else
		{
			float mult = o->o == '+' ? 1.0f : -1.0f;
			return Variant(Execute(o->right).f * mult);
		}
	}
	else if (type == ScriptNode::NOT)
	{
		Not *n = (Not*)node;

		return !Execute(n->right);
	}
	else if (type == ScriptNode::CONSTRUCTOR)
	{
		Constructor *cstr = (Constructor*)node;
		TConstructor *tc = MMASTER->get_constructor(cstr->name, cstr->params.size());
		TConstructor *c = reinterpret_cast<TConstructor*>(tc);

		switch (cstr->params.size())
		{
		case 0:
			return reinterpret_cast<CSTR_0*>(c)->operator()();
		case 1:
			return reinterpret_cast<CSTR_1*>(c)->operator()(Execute(cstr->params[0]));
		case 2:
			return reinterpret_cast<CSTR_2*>(c)->operator()(Execute(cstr->params[0]), Execute(cstr->params[1]));
		case 3:
			return reinterpret_cast<CSTR_3*>(c)->operator()(Execute(cstr->params[0]), Execute(cstr->params[1]), Execute(cstr->params[2]));
		case 4:
			return reinterpret_cast<CSTR_4*>(c)->operator()(Execute(cstr->params[0]), Execute(cstr->params[1]), Execute(cstr->params[2]), Execute(cstr->params[3]));
		default:
			T_ERROR("Invalid constructor");
			return Variant();
		}
	}
	else if (type == ScriptNode::TYPE_SPECIFIER)
	{
		TypeSpecifier *ts = reinterpret_cast<TypeSpecifier*>(node);
		Variant r = MMASTER->get_singleton(ts->referenced_type);
		return r;
	}
	return 0;
}