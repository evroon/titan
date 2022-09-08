#pragma once

class Char {
   public:
    Char(char c) { kar = c; }

    // Operators
    bool operator==(const Char &r) { return r.kar == kar; }
    bool operator==(const char &r) { return r == kar; }
    bool operator!=(const char &r) { return r != kar; }

    operator char() const { return kar; }

    // Methods
    bool istab() const { return kar == '\t'; }
    bool ischar() const {
        return (kar >= 65 && kar <= 90) || (kar >= 97 && kar <= 122);
    }
    bool isnumber() const { return kar >= 48 && kar <= 57; }
    bool isdot() const { return kar == '.'; }
    bool iswhite() const {
        return kar == ' ' || kar == '\t' || kar == '\n' || kar == '\r';
    }
    bool isoperator() const {
        return kar == '+' || kar == '-' || kar == '*' || kar == '/' ||
               kar == '=' || kar == '?' || kar == '<' || kar == '>' ||
               kar == '|' || kar == '&' || kar == '[' || kar == ']' ||
               kar == '(' || kar == ')' || kar == ',';
    }
    bool isdoubleoperator(const char right) {
        char left = kar;
        return (left == '/' && right == '/') || (left == '&' && right == '&') ||
               (left == '|' && right == '|') || (left == '+' && right == '+') ||
               (left == '-' && right == '-') ||

               (left == '<' && right == '=') || (left == '>' && right == '=') ||
               (left == '=' && right == '=') || (left == '!' && right == '=') ||

               (left == '+' && right == '=') || (left == '-' && right == '=') ||
               (left == '*' && right == '=') || (left == '/' && right == '=');
    }

    char kar;
};
