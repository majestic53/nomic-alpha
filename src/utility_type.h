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

#ifndef NOMIC_UTILITY_TYPE_H_
#define NOMIC_UTILITY_TYPE_H_

#include "../include/exception.h"

namespace nomic {

	#define NOMIC_UNILITY_HEADER "[NOMIC::UTILITY]"
#ifndef NDEBUG
	#define NOMIC_UTILITY_EXCEPTION_HEADER NOMIC_UNILITY_HEADER " "
#else
	#define NOMIC_UTILITY_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		NOMIC_UTILITY_EXCEPTION_FACE_INVALID = 0,
		NOMIC_UTILITY_EXCEPTION_FORMAT_MALFORMED,
		NOMIC_UTILITY_EXCEPTION_PATH_MALFORMED,
		NOMIC_UTILITY_EXCEPTION_PATH_NOT_FOUND,
		NOMIC_UTILITY_EXCEPTION_TYPE_INVALID,
	};

	#define NOMIC_UTILITY_EXCEPTION_MAX NOMIC_UTILITY_EXCEPTION_TYPE_INVALID

	static const std::string NOMIC_UTILITY_EXCEPTION_STR[] = {
		NOMIC_UTILITY_EXCEPTION_HEADER "Invalid block face",
		NOMIC_UTILITY_EXCEPTION_HEADER "Malformed format string",
		NOMIC_UTILITY_EXCEPTION_HEADER "Malformed file path",
		NOMIC_UTILITY_EXCEPTION_HEADER "File path does not exist",
		NOMIC_UTILITY_EXCEPTION_HEADER "Invalid block type",
		};

	#define NOMIC_UTILITY_EXCEPTION_STRING(_TYPE_) \
		(((_TYPE_) > NOMIC_UTILITY_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(NOMIC_UTILITY_EXCEPTION_STR[_TYPE_]))

	#define THROW_NOMIC_UTILITY_EXCEPTION(_EXCEPT_) \
		THROW_NOMIC_UTILITY_EXCEPTION_FORMAT(_EXCEPT_, "", "")
	#define THROW_NOMIC_UTILITY_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(NOMIC_UTILITY_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
}

#endif // NOMIC_UTILITY_TYPE_H_
