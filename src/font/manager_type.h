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

#ifndef NOMIC_FONT_MANAGER_TYPE_H_
#define NOMIC_FONT_MANAGER_TYPE_H_

#include "../../include/exception.h"

namespace nomic {

	namespace font {

		#define NOMIC_FONT_MANAGER_HEADER "[NOMIC::FONT::MANAGER]"
#ifndef NDEBUG
		#define NOMIC_FONT_MANAGER_EXCEPTION_HEADER NOMIC_FONT_MANAGER_HEADER " "
#else
		#define NOMIC_FONT_MANAGER_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NOMIC_FONT_MANAGER_EXCEPTION_CHARACTER_NOT_FOUND = 0,
			NOMIC_FONT_MANAGER_EXCEPTION_EXTERNAL,
			NOMIC_FONT_MANAGER_EXCEPTION_FONT_DUPLICATE,
			NOMIC_FONT_MANAGER_EXCEPTION_FONT_INVALID,
			NOMIC_FONT_MANAGER_EXCEPTION_FONT_NOT_FOUND,
			NOMIC_FONT_MANAGER_EXCEPTION_UNINITIALIZED,
		};

		#define NOMIC_FONT_MANAGER_EXCEPTION_MAX NOMIC_FONT_MANAGER_EXCEPTION_UNINITIALIZED

		static const std::string NOMIC_FONT_MANAGER_EXCEPTION_STR[] = {
			NOMIC_FONT_MANAGER_EXCEPTION_HEADER "Character does not exist",
			NOMIC_FONT_MANAGER_EXCEPTION_HEADER "External font exception",
			NOMIC_FONT_MANAGER_EXCEPTION_HEADER "Duplicate font id",
			NOMIC_FONT_MANAGER_EXCEPTION_HEADER "Invalid font id",
			NOMIC_FONT_MANAGER_EXCEPTION_HEADER "Font id does not exist",
			NOMIC_FONT_MANAGER_EXCEPTION_HEADER "Font manager is uninitialized",
			};

		#define NOMIC_FONT_MANAGER_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NOMIC_FONT_MANAGER_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NOMIC_FONT_MANAGER_EXCEPTION_STR[_TYPE_]))

		#define THROW_NOMIC_FONT_MANAGER_EXCEPTION(_EXCEPT_) \
			THROW_NOMIC_FONT_MANAGER_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NOMIC_FONT_MANAGER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NOMIC_FONT_MANAGER_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
	}
}

#endif // NOMIC_FONT_MANAGER_TYPE_H_
