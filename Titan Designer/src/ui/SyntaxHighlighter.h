#pragma once

#include "core/Object.h"

struct SyntaxHighlighter
{
	enum Language
	{
		C_PLUS_PLUS,
		GLSL,
		XML,
		TITANSCRIPT
	} language;

	struct Highlight
	{
		String word;
		Color color;
	};

	Array<Highlight> definitions;
};

struct SyntaxMaster
{
	SyntaxMaster();
	~SyntaxMaster();

	SyntaxHighlighter c_plus_plus;
	SyntaxHighlighter glsl;
	SyntaxHighlighter xml;
	SyntaxHighlighter titanscript;

	static SyntaxMaster* get_singleton();

	static void init();

private:
	static SyntaxMaster* singleton;

};