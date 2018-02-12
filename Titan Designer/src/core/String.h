#pragma once

#include <string>

#include "Vector.h"
#include "Array.h"
#include "TChar.h"

#define to_String(X) static_cast<String>(X)

class Real;
struct TMessage;

class String
{
public:
	String() = default;
	String(char v);
	String(char *v);
	String(const char *v);
	String(std::string v);
	String(Char v);
	String(const Real &r);
	String(unsigned i);
	String(int i);
	String(float d);

	typename std::string::iterator begin() { return src.begin(); }
	typename std::string::iterator end() { return src.end(); }

	//Operators
	bool operator==(const String & r) const;
	//bool operator==(const string &r) const;
	bool operator!=(const String & r) const;

	bool operator<(const String &r) const { return get_hash() < r.get_hash(); }

	friend std::ostream& operator<<(std::ostream &p_stream, const String &p_str);
	friend String operator+(std::string &p_l, const String &p_r);
	friend String operator+(const char* p_l, const String &p_r);

	String operator+(const String &r) const;
	String& operator+=(const String &r);
	String operator+(char r) const;
	String& operator+=(char r);
	String operator+(const char *r) const;
	String& operator+=(const char *r);
	String operator*(const int &r) const;
	String& operator*=(const int &r);

	char operator[](const int i) const;

	//Converters
	operator char() const;
	operator std::string() const;
	operator bool() const;
	operator int() const;
	operator float() const;

	//Info
	bool is_white() const;
	bool contains(const String &s) const;
	bool IsString();
	bool IsKeyword();
	bool IsVariable();
	bool IsNumber();
	bool starts_with(const String& p_str) const;
	bool ends_with(const String& p_str) const;

	//standard functions
	void append(const String &p_src);
	void replace(Char p_original, Char p_new);
	void remove(Char p_src);

	//trim
	void trim_left();
	void trim_right();
	void trim();

	int size() const;
	int length() const;
	const char* c_str() const;
	size_t get_hash() const;

	//Set
	void set(int i, char c);

	//Divide into parts
	String substr(int s, int e) const;
	Array<String> split(const char kar) const;
	
	//find
	Array<int> find(const String& p_src) const;
	Array<int> find(Char p_char) const;
	int find_first(Char p_char) const;
	int find_last(Char p_char) const;

	int count(Char p_char) const;

	String insert(int index, const String &text);
	String insert(int start, int end, char kar);
	String erase(int start, int end);

	//Parser utilities
	bool ContainsOutsideParentheses(const char elm);
	bool Contains(const String& elm);
	bool Contains(const char elm);
	int SearchChars(const String elm);
	String CharToString(const char c);
	String FloatToString(const float d);
	String IntToString(int i);
	float StringTofloat();
	int CountTabs();
	String RemoveTabs();
	
private:
	std::string src;
};

class StringName
{
public:
	StringName() : StringName(String("")) { }
	StringName(const String &p_src);
	StringName(const char *p_src) : StringName(String(p_src)) { }

	void update_hash();

	size_t get_hash() const;

	void set_source(const String &p_src);
	String get_source() const;

	//the magic
	bool operator==(const StringName &r) const;
	//operator String() const;

	operator size_t() const { return hash; }
	operator String() const { return src; }
	operator std::string() const { return src; }

	size_t hash;

private:
	String src;
};