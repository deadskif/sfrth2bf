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
#include <fstream>
#include <getopt.h>
#include <error.h>
#include <cerrno>
#include <cstdlib>

#include <config.hpp>


using namespace BFConfig;
#if !(defined INSTALL_PREFIX) && !(defined CONFIG_INSTALL_PREFIX) && !(defined CMAKE_INSTALL_PREFIX)
# warning No INSTALL_PREFIX
# define INSTALL_PREFIX "/usr"
#endif

void	Config::usage(int e) {
	std::ostream& os = e ? std::cerr : std::cout;
	os << "Usage: " << program_invocation_name << " [-h] [-d dict_file] [-o dst_file] [src_file]" << std::endl;
	exit(e);
}

Config::Config(int argc, char * argv[]) :
	dictFileName_(INSTALL_PREFIX "/share/sfrth2bf/dict.4th"),
	srcFileName_("std::cin"),
	dstFileName_("std::cout"),
	dictFile_(0),
	srcFile_(&std::cin),
	dstFile_(&std::cout)
{
	int ch;

	while((ch = getopt(argc, argv, "d:ho:v")) != EOF) {
		switch(ch) {
			case	'd':
				dictFileName_ = optarg;
				break;
			case	'h':
				usage(0);
				break;
			case	'o':
				dstFileName_ = optarg;
				if(dstFile_ != &std::cout) {
					delete dstFile_;
				}
				dstFile_ = new std::ofstream(dstFileName_);
				if(! *dstFile_) {
					error(2, errno, "Can't open distination file: %s.", dstFileName_);
				}
				//std::cerr << "Dst file: " << dstFileName << std::endl;
				break;
			case	'v':
				std::cout << "sfrth2f -- simple forth to brainfuck, version " SFRTH2BF_VERSION << std::endl;
				exit(0);
				break;
			case	'?':
			default:
				usage(1);
				break;
	
		};
	}
	argc -= optind;
	argv += optind;
	//std::cerr << "argc = " << argc << std::endl;
	if(argc > 0) {
		srcFileName_ = *argv;
		//std::cerr << "Src file = " << srcFileName << std::endl;
		srcFile_ = new std::ifstream(srcFileName_);
		if(! *srcFile_) {
			error(2, errno, "Can't open source file: %s", srcFileName_);
		}
	}
	dictFile_ = new std::ifstream(dictFileName_);
	if(! *dictFile_) {
		error(2, errno, "Can't open dictionary file: %s", dictFileName_);
		//std::cerr << errno << std::endl;
	}

}
Config::~Config() {
	if(dictFile_) {
		delete dictFile_;
	}
	if(srcFile_ != &std::cin) {
		delete srcFile_;
	}
	if(dstFile_ != &std::cout) {
		delete dstFile_;
	};
}

