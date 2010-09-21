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
#include "word.hpp"
#include "generator.hpp"
#include "exception.hpp"



namespace BFCompiler {
class CompilerErrorException : public BFException::Exception {
public:
	CompilerErrorException(const std::string & file, unsigned lineno, const std::string & message) throw(std::length_error, std::out_of_range);
	virtual ~CompilerErrorException() throw();
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

	void compile(const char* fileName, std::istream* file, bool isDict = false) throw (CompilerErrorException);

	void generate(std::ostream *);
protected:
	void startDefineNewWord() throw (CompilerErrorException);
	void stopDefineNewWord() throw (CompilerErrorException);

	void addWord(const char* s) throw (CompilerErrorException);
	void addString(const char* s) throw (CompilerErrorException);
	void addString(const std::string& s) throw (CompilerErrorException);
	void addInt(int i) throw (CompilerErrorException);

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

