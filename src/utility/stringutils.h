#pragma once

#include <sstream>
#include <string>

#include "core/vector.h"
#include "math/math.h"

class String;

class StringUtils {
   public:
    static void init();
    static void Free();
    static bool is_underscore(char kar);
    static bool IsTab(const char kar);
    static bool IsLetter(const char kar);
    static bool IsNumber(const char kar);
    static bool IsDot(const char kar);
    static bool IsOperator(const char kar);
    static bool IsDoubleOperator(const char left, const char right);
    static bool IsString(const String &src);
    static bool IsKeyword(const String &src);
    static bool IsVariable(const String &src);
    static bool IsNumber(const String &src);
    static bool Contains(const String &src, const char elm);
    static bool ContainsOutsideParentheses(const String &src, const char elm);
    static bool Contains(const String &src, const String &elm);

    static int SearchChars(const String &src, const String &elm);

    static String CharToString(const char c);
    static String FloatToString(const float f);
    static String IntToString(const int i);

    static float StringToFloat(const String &d);

    static String MultiplyString(const String &src, const int i);

    static String Trim(const String &src);
    static Vector<String> Split(const String &src, const char kar);

    static int CountTabs(const String &src);
    static String RemoveTabs(const String &src);

    static int GetLastIndexOf(const String &source, char car);

   private:
    static const String operators;
    static Vector<String> keywords;

    static const int keywordscount = 5;
};
