#pragma once

#include <string>

#include "core/data.h"
#include "utility/stringutils.h"

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

	Token lasttoken = Token("", Token::Type::UNDEF);
	Token thistoken = Token("", Token::Type::UNDEF);

	bool finishedlexing;
};