#include "stringutils.h"

#include <iostream>
#include <iomanip>

#include "core/string.h"

const String StringUtils::operators = "+-=?><|&*/[](),";
Vector<String> StringUtils::keywords;

void StringUtils::init()
{
	keywords.push_back(new String("var"));
	keywords.push_back(new String("if"));
	keywords.push_back(new String("else"));
	keywords.push_back(new String("elseif"));
	keywords.push_back(new String("func"));
	keywords.push_back(new String("return"));
	keywords.push_back(new String("while"));
}

void StringUtils::Free()
{
	keywords.clean();
}

bool StringUtils::is_underscore(char kar)
{
	return kar == '_';
}
bool StringUtils::IsTab(const char kar)
{
	return kar == '\t';
}
bool StringUtils::IsLetter(const char kar)
{
	return (kar >= 65 && kar <= 90) || (kar >= 97 && kar <= 122) || is_underscore(kar);
}
bool StringUtils::IsNumber(const char kar)
{
	return kar >= 48 && kar <= 57;
}
bool StringUtils::IsDot(const char kar)
{
	return kar == '.';
}
bool StringUtils::IsOperator(const char kar)
{
	return Contains(operators, kar);
}
bool StringUtils::IsDoubleOperator(const char left, const char right)
{
	return
		(left == '/' && right == '/') ||
		(left == '&' && right == '&') ||
		(left == '|' && right == '|') ||
		(left == '+' && right == '+') ||
		(left == '-' && right == '-') ||

		(left == '<' && right == '=') ||
		(left == '>' && right == '=') ||
		(left == '=' && right == '=') ||
		(left == '!' && right == '=') ||

		(left == '+' && right == '=') ||
		(left == '-' && right == '=') ||
		(left == '*' && right == '=') ||
		(left == '/' && right == '=');
}
bool StringUtils::IsString(const String &src)
{
	return src[0] == '"' && src[src.size() - 1] == '"';
}
bool StringUtils::IsKeyword(const String &src)
{
	for (int c = 0; c < keywordscount; c++)
		if (*keywords[c] == src)
			return true;

	return false;
}
bool StringUtils::IsVariable(const String &src)
{
	for (int c = 0; c < src.size(); c++)
		if (!IsLetter(src[c]) && !IsNumber(src[c]) && !IsDot(src[c]))
			return false;

	return true;
}
bool StringUtils::IsNumber(const String &src)
{
	for (int c = 0; c < src.size(); c++)
		if (!IsDot(src[c]) && !IsNumber(src[c]))
			return false;

	return true;
}
bool StringUtils::Contains(const String &src, const char elm)
{
	for (int c = 0; c < src.size(); c++)
		if (src[c] == elm)
			return true;
	return false;
}
bool StringUtils::ContainsOutsideParentheses(const String &src, const char elm)
{
	int level = 0;
	for (int c = 0; c < src.size(); c++)
	{
		if (src[c] == '(' || src[c] == '[') level++;
		if (src[c] == ')' || src[c] == ']') level--;

		if (src[c] == elm && level == 0)
			return true;
	}
	return false;
}
bool StringUtils::Contains(const String &src, const String &elm)
{
	int index = 0;

	for (int c = 0; c < src.size(); c++)
	{
		if (elm[index] == src[c])
		{
			if (index == elm.size() - 1)
				return true;
			index++;
		}
		else index = 0;
	}

	return false;
}
int StringUtils::SearchChars(const String &src, const String &elm)
{
	for (int c = 0; c < src.length(); c++)
		if (Contains(elm, src[c]))
			return c;

	return false;
}

String StringUtils::CharToString(const char c)
{
	if (c == ' ') return " ";
	std::stringstream ss;
	std::string s;
	ss << c;
	ss >> s;
	return s;
}
String StringUtils::FloatToString(const float f)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(3) << f;

	std::string str = ss.str();
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	str.erase(str.find_last_not_of('.') + 1, std::string::npos);

	return str;
}
String StringUtils::IntToString(int i)
{
	return std::to_string(i);
}
float StringUtils::StringToFloat(const String &s)
{
	return std::stof(s);
}

String StringUtils::MultiplyString(const String &src, const int i)
{
	String res = "";
	for (int c = 0; c < i; c++)
		res += src;

	return res;
}

String StringUtils::Trim(const String &src)
{
	String res = "";
	bool onleft = true, onright = false;
	for (int c = 0; c < src.length(); c++)
	{
		if (src[c] != '\n' && src[c] != '\t' && src[c] != ' ')
			onleft = false;
		if (!onleft && !onright)
			res += src[c];
	}

	return res;
}

Vector<String> StringUtils::Split(const String &src, const char kar)
{
	Vector<String> res;
	String buf = "";
	for (int c = 0; c < src.length(); c++)
	{
		if (src[c] != kar)	buf += src[c];
		else { res.push_back(new String(buf)); buf = ""; }
	}
	res.push_back(new String(buf));
	return res;
}

int StringUtils::CountTabs(const String &src)
{
	int count = 0;
	for (int c = 0; c < src.length(); c++)
	{
		if (src[c] == '\t')
			count++;
		else
			break;
	}
	return count;
}

String StringUtils::RemoveTabs(const String &src)
{
	String result;
	for (int c = 0; c < src.length(); c++)
		if (src[c] != '\t')
			result += src[c];
	return result;
}

int StringUtils::GetLastIndexOf(const String &source, char car)
{
	int result = -1;
	for (int c = 0; c < source.size(); c++)
		if (source[c] == car)
			result = c;
	return result;
}