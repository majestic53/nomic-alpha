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

#include <cstdarg>
#include <fstream>
#include "../include/define.h"
#include "./utility_type.h"

namespace nomic {

	#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"
	#define TIMESTAMP_LENGTH 64

	std::string 
	utility::format_as_string(
		__in const char *format,
		...
		)
	{
		std::string result;

		if(format) {
			va_list arguments;

			va_start(arguments, format);
			int length = std::vsnprintf(nullptr, 0, format, arguments);
			va_end(arguments);

			if(length > 0) {
				result.resize(++length);
				va_start(arguments, format);
				length = std::vsnprintf(&result[0], result.size(), format, arguments);
				va_end(arguments);
			}

			if(length < 0) {
				result = NOMIC_UTILITY_EXCEPTION_STRING(NOMIC_UTILITY_EXCEPTION_FORMAT_MALFORMED);
			}
		}

		return result;
	}

	std::string 
	utility::read_file(
		__in const std::string &path
		)
	{
		size_t length;
		std::string result;
		std::ifstream file;

		if(path.empty()) {
			THROW_NOMIC_UTILITY_EXCEPTION_FORMAT(NOMIC_UTILITY_EXCEPTION_PATH_MALFORMED, "Path[%u]=%p", path.size(), &path[0]);
		}

		file = std::ifstream(path.c_str(), std::ios::in);
		if(!file) {
			THROW_NOMIC_UTILITY_EXCEPTION_FORMAT(NOMIC_UTILITY_EXCEPTION_PATH_MALFORMED, "Path[%u]=%s", path.size(), STRING_CHECK(path));
		}

		file.seekg(0, std::ios::end);
		length = file.tellg();
		file.seekg(0, std::ios::beg);

		if(length) {
			result.resize(length, 0);
			file.read(&result[0], result.size());
		}

		file.close();

		return result;
	}

	time_t 
	utility::timestamp(void)
	{
		return std::time(nullptr);
	}

	std::string 
	utility::timestamp_as_string(
		__in const time_t &time
		)
	{
		size_t length;
		std::string result;

		result.resize(TIMESTAMP_LENGTH);

		length = std::strftime((char *) &result[0], TIMESTAMP_LENGTH, TIMESTAMP_FORMAT, std::localtime(&time));
		if(length) {
			result.resize(length);
		}

		return result;
	}
}
