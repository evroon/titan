#pragma once

#include "core/dictionary.h"
#include "core/map.h"
#include "core/property.h"
#include "core/signal.h"
#include "core/vector.h"
#include "tconstructor.h"

#define MMASTER MethodMaster::get_method_master()

#define REG_SINGLETON(X) \
    MMASTER->register_singleton(CLASSNAME::get_type_name_static(), X)
#define REG_CONSTANT(X) \
    MMASTER->register_constant(CLASSNAME::get_type_name_static(), {#X, X})
#define REG_SIGNAL(X) \
    MMASTER->register_signal(CLASSNAME::get_type_name_static(), X);

struct Method;

struct ConstantMember {
    StringName name;
    Variant value;
};

class ObjectCallables {
   public:
    Method *get_method_by_name(const StringName &name);
    Property *get_getsetter_by_name(const StringName &name);
    TConstructor *get_constructor_by_params(int param_count);

    void free();

    Vector<Method> methods;
    Vector<Property> properties;
    Vector<TConstructor> constructors;
    Array<ConstantMember> constants;
    Array<StringName> signal_names;

    Variant singleton;
};

class MethodMaster {
   public:
    void add_inherited_methods();
    void add_fundamental_methods();
    void add_static_functions();

    void register_constant(VariantType type, const ConstantMember &p_constant);
    void register_singleton(VariantType type, Variant p_singleton);
    void register_method(VariantType type, Method *method);
    void register_property(VariantType type, Property *getset);
    void register_constructor(VariantType type, TConstructor *cstr);
    void register_static_func(StringName name, Method *method);
    void register_signal(const VariantType &p_type, const StringName &p_signal);

    bool method_exists(VariantType type, const StringName &name);
    bool property_exists(VariantType type, const StringName &name);
    bool constructor_exists(VariantType type, int argc);
    bool signal_exists(const VariantType &p_type, const StringName &p_signal);

    Method *get_method(VariantType type, const StringName &name);
    Property *get_property(VariantType type, const StringName &name);
    TConstructor *get_constructor(VariantType type, int param_count);
    Variant get_singleton(VariantType p_type);

    Array<StringName> list_method_names(VariantType type);
    Array<StringName> list_property_names(VariantType type);
    Array<StringName> list_constructor_names(VariantType type);

    Map<StringName, Method> static_funcs;

    // static
    static void init();
    static void clean();
    static MethodMaster *get_method_master();

   private:
    Dictionary<int, ObjectCallables> object_callables;

    static MethodMaster *method_master;
};
