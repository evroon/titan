#include "core/object.h"

#include "corenames.h"
#include "variant/varianttype.h"

Object::Object(Object* base) { CHelper::set<Object, Object>(*base, *this); }

StringName Object::get_type_name() const { return "Object"; }

String Object::get_type_path() const { return get_type_name().get_source(); }

bool Object::is_type(const String& name) const { return name == "Object"; }
void* Object::get_type_ptr() const { return Object::get_type_ptr_static(); }
bool Object::is_type_ptr(void* ptr) const { return ptr == get_type_ptr(); }

VariantType Object::get_type() const { return CORE_TYPE(Object); }

StringName Object::get_type_name_static() { return "Object"; }
String Object::get_type_path_static() { return "Object"; }
void* Object::get_type_ptr_static() {
    static int ptr;
    return &ptr;
}
bool Object::is_type_static(void* ptr) { return ptr == Object::get_type_ptr_static(); }
