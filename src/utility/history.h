#pragma once

#include "core/stack.h"
#include "core/string.h"

template <typename T>
class History {
   public:
    void go_to(const T &p_current) {
        history.push(current);
        current = p_current;
        future.clear();
    }

    T go_back() {
        if (history.size() == 0) return T();

        future.push(current);

        return history.pop();
    }

    T go_forward() {
        if (future.size() == 0) return T();

        history.push(current);

        return future.pop();
    }

    void clear() {
        history.clear();
        future.clear();
    }

   private:
    Stack<T> history;
    Stack<T> future;

    T current;
};
