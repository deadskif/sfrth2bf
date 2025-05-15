
%{
/* 
 * Copyright (C) 2010 Vladimir Badaev <skif_ne@mail.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

#include "generator.hh"


/*
namespace BFScanner {
enum	TokenType {
	END,
	INT,
	START_DEFINE_WORD,
	STOP_DEFINE_WORD,
	WORD,
	STRING
}
}
*/	
#define yyterminate() return BFScanner::END
		
%}

%option 8bit yylineno
%option noyywrap warn C++

DIGIT	[0-9]

NUM	{DIGIT}+

WORD	[a-zA-Z0-9\-+/*.,~!@#$%^&_\[\]<>{}]+

STRING	\"[^\"]*\"

COMMENT "("[^")"\n]*")"

WS	[ \t\n]*

%%

{DIGIT}+		{ 
		//compiler.addInt(atoi(YYText()));
		return	BFScanner::INT;
	}

":" 			{ 
		//compiler.startDefineNewWord();
		return	BFScanner::START_DEFINE_WORD;
	}

";"			{

		//compiler.stopDefineNewWord();
		return	BFScanner::STOP_DEFINE_WORD;
	}
{WORD}			{
		//compiler.addWord(YYText());
		return	BFScanner::WORD;
	}

{STRING}		{ 
		//compiler.addString(s.c_str());
		return BFScanner::STRING;
	}

{COMMENT}
{WS}

.		{
		//std::cerr << "Error: " << lineno() << "Unrecognized character: " << YYText() << std::endl;
		return BFScanner::UNKNOWN;
	}

%%
