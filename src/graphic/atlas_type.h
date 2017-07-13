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

#ifndef NOMIC_GRAPHIC_ATLAS_TYPE_H_
#define NOMIC_GRAPHIC_ATLAS_TYPE_H_

#include "../../include/exception.h"

namespace nomic {

	namespace graphic {

		#define NOMIC_GRAPHIC_ATLAS_HEADER "[NOMIC::GRAPHIC::ATLAS]"
#ifndef NDEBUG
		#define NOMIC_GRAPHIC_ATLAS_EXCEPTION_HEADER NOMIC_GRAPHIC_ATLAS_HEADER " "
#else
		#define NOMIC_GRAPHIC_ATLAS_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NOMIC_GRAPHIC_ATLAS_EXCEPTION_ALLOCATE = 0,
			NOMIC_GRAPHIC_ATLAS_EXCEPTION_DIMENSIONS_INVALID,
			NOMIC_GRAPHIC_ATLAS_EXCEPTION_DIMENSIONS_MISMATCH,
			NOMIC_GRAPHIC_ATLAS_EXCEPTION_TYPE_INVALID,
			NOMIC_GRAPHIC_ATLAS_EXCEPTION_WIDTH_INVALID,
			NOMIC_GRAPHIC_ATLAS_EXCEPTION_WIDTH_MISMATCH,
		};

		#define NOMIC_GRAPHIC_ATLAS_EXCEPTION_MAX NOMIC_GRAPHIC_ATLAS_EXCEPTION_WIDTH_MISMATCH

		static const std::string NOMIC_GRAPHIC_ATLAS_EXCEPTION_STR[] = {
			NOMIC_GRAPHIC_ATLAS_EXCEPTION_HEADER "Failed to allocate atlas texture",
			NOMIC_GRAPHIC_ATLAS_EXCEPTION_HEADER "Invalid atlas dimensions",
			NOMIC_GRAPHIC_ATLAS_EXCEPTION_HEADER "Mismatched atlas dimensions",
			NOMIC_GRAPHIC_ATLAS_EXCEPTION_HEADER "Invalid atlas texture type",
			NOMIC_GRAPHIC_ATLAS_EXCEPTION_HEADER "Invalid atlas width",
			NOMIC_GRAPHIC_ATLAS_EXCEPTION_HEADER "Mismatched atlas width",
			};

		#define NOMIC_GRAPHIC_ATLAS_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NOMIC_GRAPHIC_ATLAS_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NOMIC_GRAPHIC_ATLAS_EXCEPTION_STR[_TYPE_]))

		#define THROW_NOMIC_GRAPHIC_ATLAS_EXCEPTION(_EXCEPT_) \
			THROW_NOMIC_GRAPHIC_ATLAS_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NOMIC_GRAPHIC_ATLAS_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NOMIC_GRAPHIC_ATLAS_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
	}
}

#endif // NOMIC_GRAPHIC_ATLAS_TYPE_H_
