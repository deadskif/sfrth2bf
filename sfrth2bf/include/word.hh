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

#ifndef WORD_HPP_

#include <ostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace BFWord {

class WordList;

typedef std::map<std::string, WordList> WordMap;

class Word
{
	public:
		Word () {};
		virtual ~Word();

		virtual void generate(std::ostream& os, const WordMap& wm) const = 0;

	private:
		
};
typedef std::shared_ptr<Word> WordPtr;



class CWord : public Word {
		std::string ww;
	public:
		CWord (std::string w);
		CWord (const char * cw);
		static WordPtr n(const std::string& s) { return WordPtr(new CWord(s)); }
		static WordPtr n(const char * cs) { return WordPtr(new CWord(cs)); }
		virtual ~CWord() {};

		virtual void generate(std::ostream& os, const WordMap& wm) const ; 

	private:
};
		

class Simple : public Word {
		char sw;
	public:
		Simple(char c) : sw(c) {};
		static WordPtr n(char c) { return WordPtr(new Simple(c)); }

		virtual ~Simple() {};

		virtual void generate(std::ostream& os, const WordMap& wm) const;
	private:

};

class Int : public Word {
		int iw;
		static void generate(int i, std::ostream& os, const WordMap& wm);
	public:
		Int(int i) : iw(i) {};
		static WordPtr n(int i) { return WordPtr(new Int(i)); }
		virtual ~Int() {};

		virtual void generate(std::ostream& os, const WordMap& wm) const;
		int value() const { return iw; }
		void value(int i) { iw = i; };
	private:
};

class String : public Word {
		std::string sw;
	public:
		String(std::string s) : sw(s) {};
		String(const char *cs) : sw(cs) {};
		static WordPtr n(const std::string& s) { return WordPtr(new String(s)); }
		static WordPtr n(const char * cs) { return WordPtr(new String(cs)); }

		virtual ~String() {};

		virtual void generate(std::ostream& os, const WordMap& wm) const;
	private:
};

class WordList {
		std::vector<WordPtr> ws;
	public:
		WordList() {}
		virtual ~WordList();

		void add(WordPtr);

		void clear() {
			ws.clear();
		}

		void generate(std::ostream& os, const WordMap& wm) const;

		static WordList nSimple(char c) {
			WordList tmp;
			tmp.add(Simple::n(c));
			return tmp;
		}
};
}; // namespace BFWord
#endif /* WORD_HPP_ */

