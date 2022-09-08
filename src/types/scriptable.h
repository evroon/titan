#pragma once

#include "core/object.h"
#include "core/signal.h"
#include "methodmaster.h"

class Variant;
class TitanScript;
class StringName;

class Scriptable : public Object {
    OBJ_DEFINITION(Scriptable, Object)

   public:
    Scriptable();
    virtual ~Scriptable();

    void set_script(TitanScript* p_script);
    TitanScript* get_script() const;

    Variant run(const StringName& name, const Arguments& args);
    bool method_exists(const StringName& name);

    Variant get(const StringName& name);
    void set(const StringName& name, const Variant& value);

    void disconnect(const StringName& p_signalname);

    void connect(const StringName& p_signalname,
                 const Connection& p_connection);
    void connect(const StringName& p_signalname, Object* p_object,
                 const StringName& p_method);
    void connect_signal(const StringName& p_in, Scriptable* p_object,
                        const StringName& p_out);

    void emit_signal(const StringName& p_name);
    void emit_signal(const StringName& p_name, Variant arg_0);

    static void bind_methods();

    Dictionary<StringName, Signal> signals;

   protected:
    bool has_script() const { return script; }

   private:
    TitanScript* script;
};
