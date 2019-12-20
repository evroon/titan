#pragma once

#include "core/Object.h"

struct SyntaxHighlighter
{
	enum Language
	{
		C_PLUS_PLUS,
		GLSL,
		XML,
		TITANSCRIPT,
		UNDEFINED
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

	static SyntaxHighlighter* get_highlighter(SyntaxHighlighter::Language p_language);

private:
	static SyntaxMaster* singleton;

};