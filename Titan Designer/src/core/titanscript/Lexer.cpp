#include "Lexer.h"

Lexer::Lexer(const String &src)
{
	source = src;
	Lex();
}

void Lexer::Free()
{
	//tokens.clean();
	//lines.clean();
	root.Free();
	source = "";
	//delete root;
}

void Lexer::LexBlock()
{
	while(index < lines.size() && !finishedlexing)
	{
		Line* l = new Line(LexLine(lines[index]));

		if (l->tokens.size() == 0 || l->StartsWith("//"))
			continue;		//Skip if line is comment or empty

		l->level = StringUtils::CountTabs(lines[index]);				//Indentation

		for (int c = 0; c < l->level; c++)								//Remove Tabs
			l->tokens.clear(0);

		parentstack.getlast()->sub.push_back(l);

		if (index + 1 == lines.size())									//Return at End of File
		{
			finishedlexing = true;
			return;
		}

		int tabcount = StringUtils::CountTabs(lines[++index]);			//#tabs of next line

		if (tabcount > l->level)										//Lex new block
		{
			parentstack.push_back(l);
			LexBlock();
		}
		else if (tabcount < l->level)									//End Block
		{
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
		if (source[c] == '/' && source[c + 1] == '/')
		{
			for(; true; c++)
			{
				if (source[c] == '\n')
					break;
			}
		}

		line += source[c]; //Fill line
		if (source[c] == '\n' || source[c] == '\r' /*|| source[c] == ';' || (source[c] == '/' && source[c + 1] == '/')*/) //Pass line
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
		if (StringUtils::IsTab((line.strings[c])[0]))
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
	Line line;
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
			buf = ".";
			line.strings.push_back(buf);
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
			line.strings.push_back(special ? buf += kar : buf);
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