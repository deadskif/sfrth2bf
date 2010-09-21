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

#include <iostream>
#include <cstring>
#include <boost/lexical_cast.hpp>
#include "compiler.hpp"


using namespace BFCompiler;
using namespace BFWord;


Compiler::Compiler() :
	cs(CS_PROGRAM),
	lexer(0),
	fname("(none)")
	//lineno(0)
{
	dictionary["_+"] = WordList::nSimple('+');
	dictionary["_-"] = WordList::nSimple('-');
	dictionary["_["] = WordList::nSimple('[');
	dictionary["_]"] = WordList::nSimple(']');
	dictionary["_,"] = WordList::nSimple(',');
	dictionary["_."] = WordList::nSimple('.');
	dictionary["_<"] = WordList::nSimple('<');
	dictionary["_>"] = WordList::nSimple('>');

	//std::cerr << "Compiler(): d[\"_+\"] == end" << (dictionary.find("_+") == dictionary.end()) << std::endl;
}
Compiler::~Compiler() {
}

void Compiler::startDefineNewWord() throw (CompilerErrorException) {
	// ":"
	if(cs != CS_PROGRAM) {
		throw CompilerErrorException(fname, lexer->lineno(), "You can't use ``:'' in word definition.");
	}
	cs = CS_WORD_DEF;
}
void Compiler::stopDefineNewWord() throw (CompilerErrorException) {
	if(cs != CS_WORD_LIST) {
		throw CompilerErrorException(fname, lexer->lineno(), "Bad use of ``;''.");
	}
	cs = CS_PROGRAM;
	dictionary[defWord] = defWordList;
	//std::cerr << "DBG: Word defined: " << defWord << ". " << std::endl;
	//std::cerr << "DBG: " << defWord << " is ";
	//defWordList.generate(std::cerr, dictionary);
	//dictionary[defWord].generate(std::cerr, dictionary); 
	//std::cerr << ". " << std::endl;
}

void Compiler::addWord(const char* w) throw (CompilerErrorException) {
	switch(cs) {
		case	CS_PROGRAM:
			program.add(CWord::n(w));
			break;
		case	CS_WORD_DEF:
			defWordList.clear();
			defWord = w;
			cs = CS_WORD_LIST;
			break;
		case	CS_WORD_LIST:
			defWordList.add(CWord::n(w));
			break;
	};
}
void Compiler::addString(const char* s) throw (CompilerErrorException) {
	addString(std::string(s));
}
void Compiler::addString(const std::string& s)throw (CompilerErrorException) {
	switch(cs) {
		case	CS_WORD_DEF:
			throw CompilerErrorException(fname, lexer->lineno(), "Bad word name: ``\"" + s + "\"''.");
			break;
		case	CS_PROGRAM:
			program.add(String::n(s));
			break;
		case	CS_WORD_LIST:
			defWordList.add(String::n(s));
			break;
	}
}
void Compiler::addInt(int i) throw (CompilerErrorException) {
	switch(cs) {
		case	CS_WORD_DEF:
			throw CompilerErrorException(fname, lexer->lineno(), "Bad word name: ``" + 
					boost::lexical_cast<std::string>(i) + "''");
			break;
		case	CS_PROGRAM:
			program.add(Int::n(i));
			break;
		case	CS_WORD_LIST:
			defWordList.add(Int::n(i));
			break;
	}
}
void Compiler::generate(std::ostream * os) {
	(*os)
		<< "********************************************************\n"
		<< "* Created with sfrth2bf                                *\n"
		<< "* (simple forth_like_language to brainfuck translator) *\n"
		<< "********************************************************\n"
		<< std::endl;
	program.generate(*os, dictionary);
	os->flush();
}

void Compiler::dumpDictionary() {
	for(WordMap::iterator i = dictionary.begin(); i != dictionary.end(); i++) {
		std::cerr << ">> " << i->first << " ::= ";
		i->second.generate(std::cerr, dictionary);
		std::cerr << std::endl;
	};
}
void Compiler::compile(const char *fname, std::istream* src, bool isDict) throw (CompilerErrorException) {
	int token;
	if(lexer) {
		delete lexer;
	};
	lexer = new yyFlexLexer(src);
	this->fname = fname;
	while((token = lexer->yylex()) != BFScanner::END){
		using	namespace BFScanner;
		const char * yytext = lexer->YYText();
		switch(token) {
			case	INT:
				addInt(atoi(yytext));
				break;
			case	START_DEFINE_WORD:
				startDefineNewWord();
				break;
			case	STOP_DEFINE_WORD:
				stopDefineNewWord();
				break;
			case	WORD:
				if(!isDict && (dictionary.find(yytext) == dictionary.end()) && (cs != CS_WORD_DEF)) {
					throw CompilerErrorException(fname, lexer->lineno(), "Undefined word `" +
						std::string(yytext) + "'.");
				}
				addWord(yytext);
				break;
			case	STRING: {
					std::string s(yytext + 1, lexer->YYLeng() - 2);
					addString(s.c_str());
				}
				break;
			case	UNKNOWN:
				throw CompilerErrorException(fname, lexer->lineno(), "Unrecognized character: " + std::string(yytext) + ".");
				break;
			default:
				throw BFException::UnrealSituationException(__FILE__, __LINE__, "Unreal default statement.");
		};
	};
}


CompilerErrorException::CompilerErrorException(
		const std::string & file,
		unsigned lineno,
		const std::string & m) throw(std::length_error, std::out_of_range) : Exception() 
{
	std::ostringstream s;
	s << file << ":" << lineno << ": Error: " << m;
	message = s.str();
};


CompilerErrorException::~CompilerErrorException() throw() {
}
//Compiler BFCompiler::compiler;
