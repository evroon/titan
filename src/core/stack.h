#pragma once

#include <stack>

template <typename T>
class Stack {
   public:
    int size() const { return to_int(stack.size()); }

    void push(const T &p_element) { stack.push(p_element); }

    T &get_top() { return stack.top(); }

    T pop() {
        T top = stack.top();
        stack.pop();
        return top;
    }

    void clear() {
        // needs improvement
        std::stack<T> temp;
        stack.swap(temp);
    }

   private:
    std::stack<T> stack;
};
