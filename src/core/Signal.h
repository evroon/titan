#pragma once

#include "types/Method.h"

class Scriptable;
class TitanScript;

struct Connection
{
	~Connection() { }

	void register_native_method(Object *p_object, const StringName &p_method_name);
	void register_script_method(Scriptable *p_scriptable, const StringName &p_method_name);
	void register_signal(Scriptable *p_scriptable, const StringName &p_signal_name);
	void register_lambda(Method* p_lambda);

	Method* method;
	Object* object;
	Scriptable* scriptable;
	StringName name;
	
	enum ConnectionType
	{
		NATIVE,
		TITANSCRIPT,
		LAMBDA,
		SIGNAL
	};

	ConnectionType type;
};

class Signal
{
public:
	Signal();
	~Signal();

	void attach_connection(const Connection &p_connection);

	//native
	void attach_native_connection(Object *p_object, const StringName &p_method_name);

	//titanscript
	void attach_script_connection(Scriptable *p_scriptable, const StringName &p_name);

	//titanscript
	void attach_signal_connection(Scriptable *p_scriptable, const StringName &p_name);

	//lambda
	void attach_lambda_connection(Method* p_lambda);

	void emit();
	void emit(Variant arg_0);

	Array<Connection> connections;
	StringName name;

private:
	void emit_connection(const Connection &p_connection) const;
	void emit_connection(const Connection &p_connection, Variant arg_0) const;
};