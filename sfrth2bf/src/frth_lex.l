
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

#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include "generator.hpp"


//BFCompiler::Compiler compiler;
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
//#define YY_DECL BFScanner::TokenType yyFlexLexer::yylex()
//using BFCompiler::compiler;
		
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
	/*
		switch(compile_state) {
			case	CS_WORDS:
				words.ws.push_back(BFWord::CWord::n(YYText()));
				break;
			case	CS_NEW_WORD:
				std::cout << "Define new word: " << YYText() << std::endl;
				def_word = YYText();
				def_words = BFWord::WordsPtr(new BFWord::Words);
				compile_state = CS_NEW_WORD_DEF;
				break;
			case	CS_NEW_WORD_DEF:
				std::cout << "New word in " << def_word << " definition: " << YYText() << std::endl;
				def_words->ws.push_back(BFWord::CWord::n(YYText()));
				break;
		}
	*/
		//compiler.addWord(YYText());
		return	BFScanner::WORD;
	}

{STRING}		{ 
	/*
		switch(compile_state) {
			case	CS_WORDS:
				words.ws.push_back(BFWord::String::n(YYText()));
				break;
			case	CS_NEW_WORD:
				std::cerr << "You can't define word with " << YYText() << " name." << std::endl;
				exit(1);
				break;
			case	CS_NEW_WORD_DEF:
				def_words->ws.push_back(BFWord::String::n(YYText()));
				break;
		}
	*/
		//std::string s(YYText() + 1, strlen(YYText()) - 2);
		////std::cerr << "DBG str parse: " << s << std::endl;
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
/*
int	yywrap() {
	exit(0);
}
*/
/*
int	main()
{

	FlexLexer* lexer = new yyFlexLexer;
	//compiler.dumpDictionary();
	lexer->yylex();

	/for(BFWord::WordPtr i = words.ws.begin(); i != words.ws.end(); i++) {
		i.generate();
	}/

	//compiler.dumpDictionary();
	compiler.generate(std::cout);
	return 0;
}
*/
