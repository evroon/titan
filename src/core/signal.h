#pragma once

#include "types/method.h"

class Scriptable;
class TitanScript;

#define LAMBDA_CONNECTION_0(X) Connection::create_from_lambda(new V_Method_0(X))

struct Connection {
    Connection() { type = UNDEF; }
    ~Connection() {}

    void register_native_method(Object* p_object, const StringName& p_method_name);
    void register_script_method(Scriptable* p_scriptable, const StringName& p_method_name);
    void register_signal(Scriptable* p_scriptable, const StringName& p_signal_name);
    void register_lambda(Method* p_lambda);

    // static versions
    static Connection create_from_native_method(Object* p_object, const StringName& p_method_name);
    static Connection create_from_script_method(Scriptable* p_scriptable,
                                                const StringName& p_method_name);
    static Connection create_from_signal(Scriptable* p_scriptable, const StringName& p_signal_name);
    static Connection create_from_lambda(Method* p_lambda);

    Method* method;
    Object* object;
    Scriptable* scriptable;
    StringName name;

    enum ConnectionType { UNDEF, NATIVE, TITANSCRIPT, LAMBDA, SIGNAL };

    ConnectionType type;
};

class Signal {
   public:
    Signal();
    ~Signal();

    void attach_connection(const Connection& p_connection);

    // native
    void attach_native_connection(Object* p_object, const StringName& p_method_name);

    // titanscript
    void attach_script_connection(Scriptable* p_scriptable, const StringName& p_name);

    // titanscript
    void attach_signal_connection(Scriptable* p_scriptable, const StringName& p_name);

    // lambda
    void attach_lambda_connection(Method* p_lambda);

    void emit();
    void emit(Variant arg_0);

    Array<Connection> connections;
    StringName name;

   private:
    void emit_connection(const Connection& p_connection) const;
    void emit_connection(const Connection& p_connection, Variant arg_0) const;
};
