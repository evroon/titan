#include "String.h"

#include "Vector.h"
#include "Array.h"

#include "TMessage.h"

#include "math/Real.h"

#include <sstream>

String::String(char *v)
{
	src = std::string(v);
}

String::String(const char *v)
{
	src = std::string(v);
}

String::String(std::string v)
{
	src = v;
}

String::String(char v)
{
	src = "";
	src += v;
}

String::String(Char v)
{
	src = "";
	src += v.kar;
}

String::String(const Real &r)
{
	src = (std::string) r.to_string();
}

String::String(unsigned i)
{
	src = std::to_string(i);
}

String::String(int i)
{
	src = std::to_string(i);
}

String::String(float d)
{
	std::stringstream ss;
	ss << d;
	src = ss.str();
}


//Operators
bool String::operator==(const String &r) const
{
	return src == r.src;
}/*
bool String::operator==(const string &r) const
{
	return src == r;
}*/
bool String::operator!=(const String &r) const
{
	return src != r.src;
}
String String::operator+(const String &r) const
{
	String res = *this;
	res += r;
	return res;
}
String& String::operator+=(const String &r)
{
	src += r.src;
	return *this;
}
String String::operator+(char r) const
{
	String res = *this;
	res += r;
	return res;
}
String& String::operator+=(char r)
{
	src += r;
	return *this;
}
String String::operator+(const char *r) const
{
	String res = *this;
	res += r;
	return res;
}
String& String::operator+=(const char *r)
{
	src += r;
	return *this;
}
String String::operator*(const int &r) const
{
	String res = *this;
	res *= r;
	return res;
}
String& String::operator*=(const int &r)
{
	String t = *this;
	for (int c = 0; c < r; c++)
		*this += t;

	return *this;
}

char String::operator[](const int i) const
{
	return src[i];
}

String::operator char() const
{
	if (size() != 1)
		T_ERROR("Invalid Conversion");
	return src[0];
}
String::operator std::string() const
{
	return src;
}
String::operator bool() const
{
	if (src == "true")
		return true;
	else if (src != "false") 
		T_ERROR("Invalid Conversion");
	return false;
}
String::operator int() const
{
	return std::stoi(c_str());
}
String::operator float() const
{
	return std::stof(c_str());
}

//Methods
const char* String::c_str() const
{
	return src.c_str();
}
size_t String::get_hash() const
{
	return std::hash<std::string>{}((std::string)src);
}
void String::set(int i, char c)
{
	src[i] = c;
}
bool String::is_white() const
{
	for (char c : src)
	{
		Char tc = c;

		if (!tc.iswhite())
			return false;
	}
	return true;
}

bool String::contains(const String &s) const
{
	int index = 0;

	for (int c = 0; c < size(); c++)
	{
		if (s[index] == src[c])
		{
			if (index == s.size() - 1)
				return true;
			index++;
		}
		else index = 0;
	}

	return false;
}
void String::append(const String & p_src)
{
	src += p_src.src;
}

void String::replace(Char p_original, Char p_new)
{
	for (int c = 0; c < size(); c++)
		if (src[c] == p_original)
			src[c] = p_new;
}

void String::remove(Char p_original)
{
	for (int c = 0; c < size(); c++)
		if (src[c] == p_original)
			src.erase(c);
}

void String::trim_left()
{

}
void String::trim_right()
{

}
void String::trim()
{
	src = StringUtils::Trim(src).src;
}
int String::size() const
{
	return length();
}
int String::length() const
{
	return to_int(src.length());
}

String String::substr(int s, int e) const
{
	return String(src.substr(s, e));
}
Array<String> String::split(const char kar) const
{
	Array<String> res;
	std::string buf = "";
	for (int c = 0; c < size(); c++)
	{
		if (src[c] != kar)	buf += src[c];
		else
		{ 
			res.push_back(buf); 
			buf = ""; 
		}
	}
	res.push_back(buf);
	return res;
}

Array<int> String::find(Char p_char) const
{
	Array<int> result;

	for (int c = 0; c < length(); c++)
	{
		if (src[c] == p_char)
			result.push_back(c);
	}

	return result;
}

int String::find_first(Char p_char) const
{
	for (int c = 0; c < length(); c++)
	{
		if (src[c] == p_char)
			return c;
	}

	return -1;
}

int String::find_last(Char p_char) const
{
	for (int c = length() - 1; c >= 0; c--)
	{
		if (src[c] == p_char)
			return c;
	}

	return -1;	
}

int String::count(Char p_char) const
{
	int result = 0;

	for (int c = 0; c < size(); c++)
	{
		if (p_char == src[c])
			result++;
	}

	return result;
}

String String::insert(int index, const String &text)
{
	return src.insert(index, text);
}
String String::insert(int start, int end, char kar)
{
	return src.insert(start, end, kar);
}
String String::erase(int start, int end)
{
	return src.erase(start, end);
}


bool String::IsString()
{
	return src[0] == '"' && src[src.length() - 1] == '"';
}
bool String::IsKeyword()
{
	return src == "var" || src == "if" || src == "else" ||
		src == "elseif" || src == "var" || src == "func" ||
		src == "return" || src == "while";
}
bool String::IsVariable()
{
	Char k(' ');
	for (int c = 0; c < size(); c++)
	{
		k = Char(src[c]);
		if (!k.ischar() && !k.isnumber() && !k.isdot())
			return false;
	}

	return true;
}
bool String::IsNumber()
{
	Char k(' ');
	for (int c = 0; c < size(); c++)
	{
		k = Char(src[c]);
		if (!k.isdot() && !k.isnumber())
			return false;
	}

	return true;
}
bool String::starts_with(const String & p_str) const
{
	for (int c = 0; c < p_str.size(); c++)
		if (src[c] != p_str[c])
			return false;

	return true;
}
bool String::ends_with(const String & p_str) const
{
	for (int c = 0; c < p_str.size(); c++)
		if (src[src.length() - 1 - c] != p_str[p_str.length() - 1 - c])
			return false;

	return true;
}
bool String::ContainsOutsideParentheses(const char elm)
{
	int level = 0;
	for (int c = 0; c < size(); c++)
	{
		if (src[c] == '(' || src[c] == '[') level++;
		if (src[c] == ')' || src[c] == ']') level--;

		if (src[c] == elm && level == 0)
			return true;
	}
	return false;
}
bool String::Contains(const String elm)
{
	int index = 0;

	for (int c = 0; c < size(); c++)
	{
		if (src[c] == elm[index])
		{
			if (index == elm.length() - 1)
				return true;
			index++;
		}
		else index = 0;
	}

	return false;
}
bool String::Contains(const char elm)
{
	for (int c = 0; c < size(); c++)
		if (src[c] == elm)
			return true;
	return false;
}
int String::SearchChars(const String elm)
{
	String str(elm);
	for (int c = 0; c < size(); c++)
	{
		if (str.Contains(src[c]))
			return c;
	}

	return false;
}

String String::CharToString(const char c)
{
	Char k(c);
	String s(k);
	return s;
}
String String::FloatToString(const float d)
{
	return std::to_string(d);
}
String String::IntToString(int i)
{
	return std::to_string(i);
}
float String::StringTofloat()
{
	return stof(src);
}

int String::CountTabs()
{
	int count = 0;
	for (int c = 0; c < size(); c++)
	{
		if (src[c] == '\t')count++;
		else break;
	}
	return count;
}

String String::RemoveTabs()
{
	String result;
	for (int c = 0; c < size(); c++)
		if (src[c] != '\t')
			result += String(src[c]);

	return result;
}

//=========================================================================
//StringName
//=========================================================================

StringName::StringName(const String &p_src)
{
	set_source(p_src);
} 

void StringName::update_hash()
{
	hash = std::hash<std::string>{}((std::string)src);
}

size_t StringName::get_hash() const
{
	return hash;
}

void StringName::set_source(const String &p_src)
{
	src = p_src;
	update_hash();
}
String StringName::get_source() const
{
	return src;
}

bool StringName::operator==(const StringName &r) const
{
	return hash == r.hash;
}

/*
StringName::operator String() const
{
	return src;
}
*/

std::ostream& operator<<(std::ostream &p_stream, const String &p_str)
{
	p_stream << p_str.src;
	return p_stream;
}

String operator+(std::string& p_l, const String& p_r)
{
	return String(p_l) + p_r.src;
}

String operator+(const char* p_l, const String& p_r)
{
	return String(p_l) + p_r;
}
