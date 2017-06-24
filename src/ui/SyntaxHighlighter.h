#pragma once

#include "core/Object.h"

class SyntaxHighlighter : public Object
{
public:
	SyntaxHighlighter();
	virtual ~SyntaxHighlighter();
};

class TitanScriptHighliter : public SyntaxHighlighter
{
public:


private:
	TitanScript* script;
};