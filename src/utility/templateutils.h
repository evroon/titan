#pragma once

#include <type_traits>

class TemplateUtils {
   public:
    TemplateUtils();
    ~TemplateUtils();

    // source:
    // http://stackoverflow.com/questions/26107041/how-can-i-determine-the-return-type-of-a-c11-member-function
    // (modified)

    template <typename T>
    struct def;

    template <typename R, typename... Args>
    struct def<R (*)(Args...)> {
        using return_type = R;
    };

    template <typename R, typename C, typename... Args>
    struct def<R (C::*)(Args...)> {
        using return_type = R;
        using base_type = C;
    };

    template <typename R, typename C, typename... Args>
    struct def<R (C::*)(Args...) const> {
        using return_type = R;
        using base_type = C;
    };

    template <typename R, typename C, typename... Args>
    struct def<R (C::*)(Args...) volatile> {
        using return_type = R;
        using base_type = C;
    };

    template <typename R, typename C, typename... Args>
    struct def<R (C::*)(Args...) const volatile> {
        using return_type = R;
        using base_type = C;
    };

    template <typename T>
    using get_return_type = typename def<T>::return_type;

    template <typename T>
    using get_base_type = typename def<T>::base_type;

    //...

    // end of source

    template <typename T>
    static bool is_void_method() {
        return std::is_same<get_return_type<T>, void>::value;
    }
};
