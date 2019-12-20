#pragma once

#include <string>

#include "core/Data.h"
#include "utility/StringUtils.h"

class Lexer
{
public:
	Lexer(const String &src);

	void Free();
	void LexBlock();
	void GetLines();
	void Lex();

	Line LexLine(const String &line);
	Line SplitLine(const String &txt);

	Line root;
	int index;

private:
	String source;

	Vector<Line> parentstack;
	Array<String> lines;

	Token lasttoken = Token("", 0);
	Token thistoken = Token("", 0);

	bool finishedlexing;
};