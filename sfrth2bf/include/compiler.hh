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

#ifndef COMPILER_HPP_

#define COMPILER_HPP_


#include <string>
#include <ostream>
#include <exception>
//#define YY_DECL BFScanner::TokenType yyFlexLexer::yylex()

#include <FlexLexer.h>
#include "word.hh"
#include "generator.hh"
#include "exception.hh"



namespace BFCompiler {
class CompilerErrorException : public BFException::Exception {
public:
	CompilerErrorException(const std::string & file, unsigned lineno, const std::string & message);
	virtual ~CompilerErrorException();
};


class Compiler {
	BFWord::WordList program;
	enum CompilerState {
		CS_PROGRAM,
		CS_WORD_DEF,
		CS_WORD_LIST
	};
	CompilerState cs;
	BFWord::WordMap dictionary;

	BFWord::WordList defWordList;
	std::string defWord;

	FlexLexer *lexer;
	const char *fname;
public:
	Compiler();
	~Compiler();

	void compile(const char* fileName, std::istream* file, bool isDict = false);

	void generate(std::ostream *);
protected:
	void startDefineNewWord();
	void stopDefineNewWord();

	void addWord(const char* s);
	void addString(const char* s);
	void addString(const std::string& s);
	void addInt(int i);

/*
	void newLine() {
		lineno++;
	}
*/

	void dumpDictionary();
};
//extern Compiler compiler;
}; // namespaec BFCompiler

#endif /* COMPILER_HPP_ */

