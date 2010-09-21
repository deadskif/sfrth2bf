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

#include "word.hpp"
using namespace BFWord;
using namespace std;

Word::~Word(){};

WordList::~WordList(){};

void WordList::add(WordPtr w) {
	ws.push_back(w);
}

CWord::CWord(std::string s) : ww(s) {}
CWord::CWord(const char *cs) : ww(cs) {}

void CWord::generate(std::ostream& os, const WordMap& wm) const {
	//std::cerr << "DBG gen CWord:" << ww << std::endl;
	if(wm.find(ww) == wm.end()) {
		cerr << "No such word: " << ww << endl;
		exit(1);
	};
	wm.at(ww).generate(os, wm);
	//os << endl;
}
void Simple::generate(std::ostream& os, const WordMap& wm) const {
	//std::cerr << "DBG gen Simple: `" << sw << "'" << endl;
	os << sw;
}
void Int::generate(std::ostream& os, const WordMap& wm) const {
	//os << "IntWord(" << iw << ")" << endl;
	//std::cerr << "DBG gen Int:" << iw << endl;
	Int::generate(iw, os, wm);
}
void Int::generate(int i, std::ostream& os, const WordMap& wm) {
	/*
	if(i < 0) {
		std::cerr << "Error: negative numbers is not released." << std::endl;
		exit(1);
	} else if (i == 0) {
		os << '>';
		wm.at("ZERO").generate(os, wm);
	} else if (i < 11) {
		Int::generate(0, os, wm);
		for(int j = 0; j < i; j++) {
			os << '+';
		}
	} else {
		Int::generate(i / 10, os, wm);
		Int::generate(10, os, wm);
		wm.at("*").generate(os, wm);
		Int::generate(i % 10, os, wm);
		wm.at("+").generate(os, wm);
	}
	*/
	int a = 0, b = 0, c = 0;
	int m = 11, n = 11;
	if(i < 0) {
		std::cerr << "Error: negative numbers is not released." << std::endl;
		exit(1);
	} else if(i == 0) {
		os << '>';
		wm.at("ZERO").generate(os, wm);
	} else if(i < 14) {
		//std::cerr << "What a fuck?" << std::endl;
		Int::generate(0, os, wm);
		for(int j = 0; j < i; j++) {
			os << '+';
		}
		//os.flush();
	} else if(i <= (11*12)) {
		// Hint: i = a + m*b
		b = i / m;
		a = i - m*b;
		//std::cerr << "a = " << a << ", b = " << b << ", m = " << m << std::endl;
		
		Int::generate(a, os, wm);
		Int::generate(b, os, wm);
		os << "[<";
		for(int j = 0; j < m; j++) {
			os << '+';
		}
		os << ">-]<";
		os << std::endl;
	} else if(i <= 255){
		// Hint: i = a + m*b + n*c
		//a = 11;
		b = m;
		c = (i - (b*m)) / n;
		a = i - (m*b + n*c);
		//std::cerr << "a = " << a << ", b = " << b << ", c = " << c << ", m = " << m << ", n = " << n << std::endl;
		
		Int::generate(a, os, wm);
		Int::generate(b, os, wm);
		Int::generate(c, os, wm);
		os << "<[<";
		for(int j = 0; j < m; j++) {
			os << '+';
		}
		os << ">-]>[<<";
		for(int j = 0; j < n; j++) {
			os << '+';
		}
		os << ">>-]<<";
		os << std::endl;
	}


}
void String::generate(std::ostream& os, const WordMap& wm) const {
	//std::cerr << "DBG gen String`" << sw << "\'" << endl;
	Int tmp(0);
	tmp.generate(os, wm);
	for(std::string::const_reverse_iterator i = sw.rbegin(); i != sw.rend(); i++) {
		tmp.value(*i);
		tmp.generate(os, wm);
	}
	os << std::endl;
}

void WordList::generate(std::ostream& os, const WordMap& wm) const {
	for(std::vector<WordPtr>::const_iterator i = ws.begin(); i != ws.end(); i++) {
		(*i)->generate(os, wm);
	};
	//os << std::endl;
}
