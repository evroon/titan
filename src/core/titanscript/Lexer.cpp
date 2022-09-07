#include "lexer.h"

Lexer::Lexer(const String &src)
{
	parentstack = Vector<Line>();
	lines = Array<String>();

	Line root = Line();
	index = 0;
	finishedlexing = false;
	source = src;
	Lex();
}

void Lexer::Free()
{
	parentstack.clean();
}

void Lexer::LexBlock()
{
	while (index < lines.size())
	{
		Line* l = new Line(LexLine(lines[index]));

		// Skip if line is comment or empty
		if (l->tokens.size() == 0 || l->StartsWith("//"))
			continue;

		// Indentation
		l->level = StringUtils::CountTabs(lines[index]);

		// Remove Tabs
		for (int c = 0; c < l->level; c++)
			l->tokens.clear(0);

		parentstack.getlast()->sub.push_back(l);

		// Return at End of File
		if (index == lines.size() - 1)
			return;

		// Number of tabs of next line
		int tabcount = StringUtils::CountTabs(lines[++index]);

		// Lex new block
		if (tabcount > l->level)
		{
			parentstack.push_back(l);
			LexBlock();
		}
		else if (tabcount < l->level)
		{
			// End Block
			for (int c = 0; c < l->level - tabcount; c++)
				parentstack.removelast();

			return;
		}
	}
}

void Lexer::GetLines()
{
	String line = "";
	for (int c = 0; c < source.length(); c++)
	{
		if (source[c] == '/' && c < source.length() - 1 && -source[c + 1] == '/')
		{
			for(; true; c++)
			{
				if (source[c] == '\n')
					break;
			}
		}

		// Fill line
		line += source[c];

		// Pass line
		if (source[c] == '\n' || source[c] == '\r')
		{
			if (StringUtils::Trim(line) != "")
				lines.push_back(line);

			line = "";
		}
	}
}

void Lexer::Lex()
{
	GetLines();
	root.level = -1;

	parentstack.push_back(&root);	//Root is the first Parent
	LexBlock();
	parentstack.clear();
}

Line Lexer::LexLine(const String &txt)
{
	Line line = SplitLine(txt);

	for (int c = 0; c < line.strings.size(); c++)
	{
		if (StringUtils::IsTab(line.strings[c][0]))
			line.tokens.push_back(Token(line.strings[c], Token::TAB));		//Add Tab
		else if (StringUtils::IsNumber(line.strings[c]))
			line.tokens.push_back(Token(line.strings[c], Token::NUMBER));	//Add Number
		else if (StringUtils::IsKeyword(line.strings[c]))
			line.tokens.push_back(Token(line.strings[c], Token::KEYWORD));	//Add Keyword
		else if (StringUtils::IsVariable(line.strings[c]))
			line.tokens.push_back(Token(line.strings[c], Token::WORD));		//Add Word
		else if (StringUtils::IsOperator((line.strings[c])[0]))
			line.tokens.push_back(Token(line.strings[c], Token::OPERATOR));	//Add Operator
		else if (StringUtils::IsString(line.strings[c]))
			line.tokens.push_back(Token(line.strings[c], Token::STRING));	//Add String
	}
	return line;
}

Line Lexer::SplitLine(const String &txt)
{
	char prev = '\n';
	String buf = "";
	Line line = Line();

	for (int c = 0; c < txt.length(); c++)
	{
		char kar = txt[c];

		bool prevdot = prev == '.';
		bool thisdot = kar == '.';

		bool prevnumber = StringUtils::IsNumber(prev);
		bool thisnumber = StringUtils::IsNumber(kar);

		bool prevletter = StringUtils::IsLetter(prev) || prevnumber || (thisdot && thisnumber) || (prevdot && thisnumber);
		bool thisletter = StringUtils::IsLetter(kar) || thisnumber || (thisdot && prevnumber) || (prevdot && thisnumber);

		bool prevop = StringUtils::IsOperator(prev);
		bool thisop = StringUtils::IsOperator(kar);

		bool special = StringUtils::IsDoubleOperator(prev, kar);

		if (thisdot && !StringUtils::IsNumber(prev))
		{
			line.strings.push_back(buf);
			line.strings.push_back(".");
			buf = "";
			prev = kar;
			continue;
		}

		if (kar == '/' && c < txt.length() - 1 && txt[c + 1] == '/')
			return line;

		if (kar == '\t')
			line.strings.push_back("\t");
		else if (kar == '"')																	//Fill String
		{
			if (buf != "")
				line.strings.push_back(buf);

			buf = "\"";
			for (c++; c < txt.length(); c++)
				if (txt[c] != '"')
					buf += txt[c];
				else break;

			line.strings.push_back(buf + "\"");
			buf = "";
		}
		if (prevletter && thisletter)															//Continue filling
			buf += kar;
		else if (prevop && thisop)																//Check for double operators
		{
			line.strings.push_back(special ? buf + kar : buf);
			if (special)	buf = "";
			else			buf = kar;
		}
		else if ((prevletter && !thisletter) || (prevop && !thisop) || (prevletter && thisop) || (prevdot && thisop))	//End filling
		{
			if (buf != "")
			{
				line.strings.push_back(buf);
				buf = kar;
			}
		}
		else if ((!prevletter && thisletter) || (!prevop && thisop))							//Start filling
			buf = kar;
		else if (!prevletter && !thisletter)													//Pass operator
			if (kar != '\n'  && kar != ' ' && buf != "")
				line.strings.push_back(buf);

		prev = kar;
	}
	return line;
}