#include "Scriptable.h"

#include "core/variant/Variant.h"
#include "core/titanscript/TitanScript.h"
#include "core/Object.h"

Scriptable::Scriptable()
{
}


Scriptable::~Scriptable()
{
}

void Scriptable::set_script(TitanScript *p_script)
{
	script = p_script;
	if (script)
		script->Extend(this);
}

TitanScript* Scriptable::get_script() const
{
	return script;
}

Variant Scriptable::run(const StringName &name, const Arguments& args)
{
	Method *m = MMASTER->get_method(get_type(), name);

	if (!m && script)
		return script->RunFunction(name, args);
	else if (m)
		return m->operator()(args);
	else
	{
		T_ERROR("Invalid function call");
		return NULL_VAR;
	}
}

bool Scriptable::method_exists(const StringName &name)
{
	return has_script() && script->FunctionExists(name);
}

Variant Scriptable::get(const StringName &name)
{
	Property *p = MMASTER->get_property(get_type(), name);
	return p->get->operator() (Variant(this));
}

void Scriptable::set(const StringName &name, const Variant &value)
{
	Property *p = MMASTER->get_property(get_type(), name);
	p->set->operator() (Variant(this), Variant(value));
}

void Scriptable::connect(const StringName & p_signalname, const Connection & p_connection)
{
	if (!MMASTER->signal_exists(get_type(), p_signalname))
	{
		T_ERROR("Signal does not exist: " + p_signalname + " in type: " + get_type_name() + " for connection: " + p_connection.name);
		return;
	}

	if (signals.contains(p_signalname))
		signals[p_signalname].attach_connection(p_connection);
	else
	{
		Signal signal;
		signal.name = p_signalname;
		signal.attach_connection(p_connection);

		signals.set(p_signalname, signal);
	}
}

void Scriptable::connect(const StringName & p_signalname, Object* p_object, const StringName& p_method)
{
	if (!p_object)
	{
		T_ERROR("p_object is null for signal:  " + p_signalname);
		return;
	}

	if (!MMASTER->signal_exists(get_type(), p_signalname))
	{
		T_ERROR("Signal does not exist: " + p_signalname + " in type: " + get_type_name() + " for method: " + p_method);
		return;
	}

	Connection connection;
	connection.register_native_method(p_object, p_method);

	if (signals.contains(p_signalname))
		signals[p_signalname].attach_connection(connection);
	else
	{
		Signal signal;
		signal.name = p_signalname;
		signal.attach_connection(connection);

		signals.set(p_signalname, signal);
	}
}

void Scriptable::connect_signal(const StringName& p_in, Scriptable* p_object, const StringName& p_out)
{
	if (!MMASTER->signal_exists(get_type(), p_in))
	{
		T_ERROR("Signal does not exist: " + p_in + " in type: " + get_type_name() + " for method: " + p_out);
		return;
	}

	Connection connection;
	connection.register_signal(p_object, p_out);

	if (signals.contains(p_in))
		signals[p_in].attach_connection(connection);
	else
	{
		Signal signal;
		signal.name = p_in;
		signal.attach_connection(connection);

		signals.set(p_in, signal);
	}
}

void Scriptable::emit_signal(const StringName & p_name)
{
	signals[p_name].emit();
}

void Scriptable::emit_signal(const StringName & p_name, Variant arg_0)
{
	signals[p_name].emit(arg_0);
}

#undef CLASSNAME
#define CLASSNAME Scriptable

void Scriptable::bind_methods()
{
	REG_PROPERTY(script);
}
