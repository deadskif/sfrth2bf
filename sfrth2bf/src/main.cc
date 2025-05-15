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
#include <FlexLexer.h>
#include "config.hh"
#include "compiler.hh"

int main (int argc, char * argv[])
{
	BFConfig::Config config(argc, argv);
	BFCompiler::Compiler c;
	try {	
		c.compile(config.dictFileName(),
				config.dictFile(), true);
		c.compile(config.srcFileName(),
				config.srcFile());
		c.generate(config.dstFile());
	}catch(BFCompiler::CompilerErrorException & e) {
		std::cerr << e.what() << std::endl;
		exit(2);
	}catch(BFException::Exception & e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	return 0;
}
