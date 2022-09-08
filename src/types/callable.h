#pragma once

#include <string>

#include "core/array.h"
#include "core/variant/variant.h"
#include "core/variant/varianttype.h"

typedef Array<Variant> Arguments;

struct Parameter {
    Parameter(const StringName& p_name) : name(p_name) {}

    StringName name;

    bool operator==(const Parameter& r) const { return name == r.name; }
    bool operator!=(const Parameter& r) const { return !(name == r.name); }
};

struct ParameterNames {
    ParameterNames() {}

    ParameterNames(const Array<Parameter>& p_formal_params) {
        formal_params = p_formal_params;
    }
    Array<Parameter> formal_params;

    bool operator==(const ParameterNames& r) {
        return formal_params == r.formal_params;
    }
    bool operator!=(const ParameterNames& r) {
        return !(formal_params == r.formal_params);
    }
};

typedef Array<String> ParameterTypes;
typedef VariantType ReturnType;

struct Callable {
    bool test;
};
