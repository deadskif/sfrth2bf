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
#ifndef EXCEPTION_HPP_

#define EXCEPTION_HPP_

#include <string>
#include <exception>
#include <stdexcept>
#include <sstream>


namespace BFException {
class Exception : public std::exception
{
	public:
		virtual ~Exception() throw() {};
		const char *what() const throw() { return message.c_str(); };

	protected:
		std::string message;
		Exception () throw(std::length_error, std::out_of_range) {};
		Exception (const std::string & m) throw (std::length_error, std::out_of_range) : message(m) {};
		
};
class UnrealSituationException : Exception {
	public:
		UnrealSituationException (const char * file, unsigned line, const char * m)
			throw(std::length_error, std::out_of_range)
		{
			std::ostringstream s;
			s << "UnrealSituationException: " << file << ":" << line << ": " << m;
			message = s.str();
		};
		virtual ~UnrealSituationException() throw () {}
};
};

#endif /* EXCEPTION_HPP_ */

