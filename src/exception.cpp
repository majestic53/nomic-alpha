/**
 * Nomic
 * Copyright (C) 2017 David Jolly
 *
 * Nomic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nomic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/exception.h"
#include "../include/trace.h"

namespace nomic {

	exception::exception(void) :
		std::runtime_error(std::string()),
		m_empty(true),
		m_line(0)
	{
		return;
	}

	exception::exception(
		__in const std::string &message,
		__in const std::string &file,
		__in const std::string &function,
		__in size_t line
		) :
			std::runtime_error(message),
			m_empty(false),
			m_file(file),
			m_function(function),
			m_line(line)
	{
		return;
	}

	exception::exception(
		__in const exception &other
		) :
			std::runtime_error(other),
			m_empty(other.m_empty),
			m_file(other.m_file),
			m_function(other.m_function),
			m_line(other.m_line)
	{
		return;
	}

	exception::~exception(void)
	{
		return;
	}

	exception &
	exception::operator=(
		__in const exception &other
		)
	{

		if(this != &other) {
			std::runtime_error::operator=(other);
			m_empty = other.m_empty;
			m_file = other.m_file;
			m_function = other.m_function;
			m_line = other.m_line;
		}

		return *this;
	}

	void 
	exception::clear(void)
	{
		std::runtime_error::operator=(std::runtime_error(std::string()));
		m_empty = true;
		m_file.clear();
		m_function.clear();
		m_line = 0;
	}

	bool 
	exception::empty(void) const
	{
		return m_empty;
	}

	std::string 
	exception::file(void) const
	{
		return m_file;
	}

	std::string 
	exception::function(void) const
	{
		return m_function;
	}

	void 
	exception::generate(
		__in const std::string &message,
		__in const std::string &file,
		__in const std::string &function,
		__in size_t line,
		__in const std::string &format
		)
	{
		std::stringstream result;

		result << message;

		if(!format.empty()) {

			if(!result.str().empty()) {
				result << ": ";
			}

			result << format;
		}

		TRACE_EXCEPTION(result.str(), file, function, line);
		throw nomic::exception(result.str(), file, function, line);
	}

	size_t 
	exception::line(void) const
	{
		return m_line;
	}

	std::string 
	exception::to_string(
		__in_opt bool verbose
		) const
	{
		std::stringstream result;

		result << std::runtime_error::what();

		if(verbose) {

			if(!result.str().empty()) {
				result << " ";
			}

			result << "(";

			if(!m_function.empty()) {
				result << m_function << ":";
			}

			if(!m_file.empty()) {
				result << m_file << "@";
			}

			result << m_line << ")";
		}

		return result.str();
	}
}
