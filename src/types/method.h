#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "callable.h"
#include "core/variant/variant.h"

// define defines
#define CLASSNAME
#define COMMA ,
#define DCAST dynamic_cast<CLASSNAME*>
#define CAST reinterpret_cast<CLASSNAME*>
#define INVOKE0(METHOD) return CAST()(in.operator Object*())->METHOD()
#define INVOKE(METHOD, ARGS) return CAST()(in.operator Object*())->METHOD(ARGS)

// base classes for method
struct Method : Callable {
    Method() {}

    virtual Variant operator()(const Array<Variant>& args) { return Variant(); }

    StringName name = "";
    bool returns_variant;
    bool is_const;
    int arg_count;
    ParameterNames param_names;
    ParameterTypes param_types;
    VariantType inherits_from;
};

struct VoidMethod : public Method {
    VoidMethod() { returns_variant = false; }
};

struct ReturnMethod : public Method {
    ReturnMethod() { returns_variant = true; }
    ReturnType return_type;
};

// deprecated
// define functor defines for functions that return Variant or void
#define BUILD_VOID_FUNCTOR(ARGC, TYPE, PARAMS, ARGS)      \
    struct V_Method_##ARGC : public VoidMethod {          \
        V_Method_##ARGC() { arg_count = ARGC; }           \
        V_Method_##ARGC(std::function<void(TYPE)> p_func) \
            : V_Method_##ARGC() {                         \
            func = p_func;                                \
        }                                                 \
        void operator()(PARAMS) { func(ARGS); }           \
        Variant invoke_return(PARAMS) {                   \
            func(ARGS);                                   \
            return Variant();                             \
        }                                                 \
        std::function<void(TYPE)> func;                   \
    }

#define BUILD_RET_FUNCTOR(ARGC, TYPE, PARAMS, ARGS)          \
    struct R_Method_##ARGC : public ReturnMethod {           \
        R_Method_##ARGC() { arg_count = ARGC; }              \
        R_Method_##ARGC(std::function<Variant(TYPE)> p_func) \
            : R_Method_##ARGC() {                            \
            func = p_func;                                   \
        }                                                    \
        Variant operator()(PARAMS) { return func(ARGS); }    \
        std::function<Variant(TYPE)> func;                   \
    }

// VOID

#define VAR Variant

struct V_Method_0 : public VoidMethod {
    V_Method_0() { arg_count = 0; }
    V_Method_0(std::function<void()> p_func) : V_Method_0() { func = p_func; }
    void operator()() { func(); }
    Variant invoke_return() {
        func();
        return Variant();
    }
    std::function<void()> func;
};

struct V_Method_1 : public VoidMethod {
    V_Method_1() { arg_count = 1; }
    V_Method_1(std::function<void(VAR)> p_func) : V_Method_1() {
        func = p_func;
    }
    void operator()(VAR arg_0) { func(arg_0); }
    Variant invoke_return(VAR arg_0) {
        func(arg_0);
        return Variant();
    }
    std::function<void(VAR)> func;
};

struct V_Method_2 : public VoidMethod {
    V_Method_2() { arg_count = 2; }
    V_Method_2(std::function<void(VAR, VAR)> p_func) : V_Method_2() {
        func = p_func;
    }
    void operator()(VAR arg_0, VAR arg_1) { func(arg_0, arg_1); }
    Variant invoke_return(VAR arg_0, VAR arg_1) {
        func(arg_0, arg_1);
        return Variant();
    }
    std::function<void(VAR, VAR)> func;
};

struct V_Method_3 : public VoidMethod {
    V_Method_3() { arg_count = 3; }
    V_Method_3(std::function<void(VAR, VAR, VAR)> p_func) : V_Method_3() {
        func = p_func;
    }
    void operator()(VAR arg_0, VAR arg_1, VAR arg_2) {
        func(arg_0, arg_1, arg_2);
    }
    Variant invoke_return(VAR arg_0, VAR arg_1, VAR arg_2) {
        func(arg_0, arg_1, arg_2);
        return Variant();
    }
    std::function<void(VAR, VAR, VAR)> func;
};

struct V_Method_4 : public VoidMethod {
    V_Method_4() { arg_count = 4; }
    V_Method_4(std::function<void(VAR, VAR, VAR, VAR)> p_func) : V_Method_4() {
        func = p_func;
    }
    void operator()(VAR arg_0, VAR arg_1, VAR arg_2, VAR arg_3) {
        func(arg_0, arg_1, arg_2, arg_3);
    }
    Variant invoke_return(VAR arg_0, VAR arg_1, VAR arg_2, VAR arg_3) {
        func(arg_0, arg_1, arg_2, arg_3);
        return Variant();
    }
    std::function<void(VAR, VAR, VAR, VAR)> func;
};

// WITH RETURN

struct R_Method_0 : public ReturnMethod {
    R_Method_0() { arg_count = 0; }
    R_Method_0(std::function<VAR()> p_func) : R_Method_0() { func = p_func; }
    VAR operator()() { return func(); }
    VAR invoke_return() {
        func();
        return func();
    }
    std::function<VAR()> func;
};

struct R_Method_1 : public ReturnMethod {
    R_Method_1() { arg_count = 1; }
    R_Method_1(std::function<VAR(VAR)> p_func) : R_Method_1() { func = p_func; }
    VAR operator()(VAR arg_0) { return func(arg_0); }
    VAR invoke_return(VAR arg_0) { return func(arg_0); }
    std::function<VAR(VAR)> func;
};

struct R_Method_2 : public ReturnMethod {
    R_Method_2() { arg_count = 2; }
    R_Method_2(std::function<VAR(VAR, VAR)> p_func) : R_Method_2() {
        func = p_func;
    }
    VAR operator()(VAR arg_0, VAR arg_1) { return func(arg_0, arg_1); }
    VAR invoke_return(VAR arg_0, VAR arg_1) { return func(arg_0, arg_1); }
    std::function<VAR(VAR, VAR)> func;
};

struct R_Method_3 : public ReturnMethod {
    R_Method_3() { arg_count = 3; }
    R_Method_3(std::function<VAR(VAR, VAR, VAR)> p_func) : R_Method_3() {
        func = p_func;
    }
    VAR operator()(VAR arg_0, VAR arg_1, VAR arg_2) {
        return func(arg_0, arg_1, arg_2);
    }
    VAR invoke_return(VAR arg_0, VAR arg_1, VAR arg_2) {
        return func(arg_0, arg_1, arg_2);
    }
    std::function<VAR(VAR, VAR, VAR)> func;
};

struct R_Method_4 : public ReturnMethod {
    R_Method_4() { arg_count = 4; }
    R_Method_4(std::function<VAR(VAR, VAR, VAR, VAR)> p_func) : R_Method_4() {
        func = p_func;
    }
    VAR operator()(VAR arg_0, VAR arg_1, VAR arg_2, VAR arg_3) {
        return func(arg_0, arg_1, arg_2, arg_3);
    }
    VAR invoke_return(VAR arg_0, VAR arg_1, VAR arg_2, VAR arg_3) {
        return func(arg_0, arg_1, arg_2, arg_3);
    }
    std::function<VAR(VAR, VAR, VAR, VAR)> func;
};
