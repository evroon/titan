#include "core/signal.h"

#include "core/titanscript/titanscript.h"
#include "types/methodmaster.h"
#include "types/scriptable.h"
#include "types/typemanager.h"

//=========================================================================
// Connection
//=========================================================================

void Connection::register_native_method(Object* p_object, const StringName& p_method_name) {
    type = NATIVE;

    object = p_object;
    method = reinterpret_cast<V_Method_1*>(MMASTER->get_method(object->get_type(), p_method_name));

    if (!method) {
        String type = object->get_type_name().get_source();
        T_ERROR("Could not find method: " + p_method_name.get_source() +
                " for object of type: " + type);
    }
}

void Connection::register_script_method(Scriptable* p_scriptable, const StringName& p_method_name) {
    type = TITANSCRIPT;

    scriptable = p_scriptable;
    name = p_method_name;
}

void Connection::register_signal(Scriptable* p_scriptable, const StringName& p_signal_name) {
    type = SIGNAL;

    scriptable = p_scriptable;
    name = p_signal_name;
}

void Connection::register_lambda(Method* p_lambda) {
    type = LAMBDA;
    method = p_lambda;
}

Connection Connection::create_from_native_method(Object* p_object,
                                                 const StringName& p_method_name) {
    Connection c;
    c.register_native_method(p_object, p_method_name);
    return c;
}

Connection Connection::create_from_script_method(Scriptable* p_scriptable,
                                                 const StringName& p_method_name) {
    Connection c;
    c.register_signal(p_scriptable, p_method_name);
    return c;
}

Connection Connection::create_from_signal(Scriptable* p_scriptable,
                                          const StringName& p_signal_name) {
    Connection c;
    c.register_signal(p_scriptable, p_signal_name);
    return c;
}

Connection Connection::create_from_lambda(Method* p_lambda) {
    Connection c;
    c.register_lambda(p_lambda);
    return c;
}

//=========================================================================
// Signal
//=========================================================================

Signal::Signal() {}

Signal::~Signal() {}

void Signal::attach_connection(const Connection& p_connection) {
    connections.push_back(p_connection);
}

void Signal::attach_native_connection(Object* p_object, const StringName& p_method_name) {
    Connection connection;
    connection.register_native_method(p_object, p_method_name);

    connections.push_back(connection);
}

void Signal::attach_script_connection(Scriptable* p_scriptable, const StringName& p_name) {
    Connection connection;
    connection.register_script_method(p_scriptable, p_name);

    connections.push_back(connection);
}

void Signal::attach_signal_connection(Scriptable* p_scriptable, const StringName& p_name) {
    Connection connection;
    connection.register_signal(p_scriptable, p_name);

    connections.push_back(connection);
}

void Signal::attach_lambda_connection(Method* p_lambda) {
    Connection connection;
    connection.register_lambda(p_lambda);

    connections.push_back(connection);
}

void Signal::emit() {
    for (Connection& connection : connections) emit_connection(connection);
}

void Signal::emit(Variant arg_0) {
    for (Connection& connection : connections) emit_connection(connection, arg_0);
}

void Signal::emit_connection(const Connection& p_connection) const {
    Connection* c = const_cast<Connection*>(&p_connection);

    if (!p_connection.method) {
        T_ERROR("connection " + p_connection.name.get_source() +
                " has no method for signal: " + name.get_source());
        return;
    }

    if (p_connection.type == Connection::NATIVE) {
        if (p_connection.method->arg_count == 1) {
            V_Method_1* m = reinterpret_cast<V_Method_1*>(p_connection.method);
            m->operator()(p_connection.object);
        } else
            T_ERROR("argument count does not match");
    } else if (p_connection.type == Connection::LAMBDA) {
        V_Method_0* m = reinterpret_cast<V_Method_0*>(p_connection.method);
        m->operator()();
    } else if (p_connection.type == Connection::TITANSCRIPT)
        p_connection.scriptable->run(p_connection.name, Arguments());
    else if (p_connection.type == Connection::SIGNAL)
        p_connection.scriptable->emit_signal(p_connection.name);
    else
        (*c).method->operator()({});
}

void Signal::emit_connection(const Connection& p_connection, Variant arg_0) const {
    if (!p_connection.method) {
        T_ERROR("connection " + p_connection.name.get_source() +
                " has no method for signal: " + name.get_source());
        return;
    }

    if (p_connection.type == Connection::NATIVE) {
        if (p_connection.method->arg_count == 2) {
            V_Method_2* m = reinterpret_cast<V_Method_2*>(p_connection.method);
            m->operator()(p_connection.object, arg_0);
        } else
            T_ERROR("argument count does not match");
    } else if (p_connection.type == Connection::LAMBDA) {
        V_Method_1* m = reinterpret_cast<V_Method_1*>(p_connection.method);
        m->operator()(arg_0);
    } else if (p_connection.type == Connection::TITANSCRIPT)
        p_connection.scriptable->run(p_connection.name, Arguments(arg_0));
    else if (p_connection.type == Connection::SIGNAL)
        p_connection.scriptable->emit_signal(p_connection.name, Arguments(arg_0));
}
