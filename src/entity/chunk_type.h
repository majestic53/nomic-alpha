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

#ifndef NOMIC_ENTITY_CHUNK_TYPE_H_
#define NOMIC_ENTITY_CHUNK_TYPE_H_

#include "../../include/exception.h"

namespace nomic {

	namespace entity {

		#define NOMIC_ENTITY_CHUNK_HEADER "[NOMIC::ENTITY::CHUNK]"
#ifndef NDEBUG
		#define NOMIC_ENTITY_CHUNK_EXCEPTION_HEADER NOMIC_ENTITY_CHUNK_HEADER " "
#else
		#define NOMIC_ENTITY_CHUNK_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NOMIC_ENTITY_CHUNK_EXCEPTION_NONE = 0,
		};

		#define NOMIC_ENTITY_CHUNK_EXCEPTION_MAX NOMIC_ENTITY_CHUNK_EXCEPTION_NONE

		static const std::string NOMIC_ENTITY_CHUNK_EXCEPTION_STR[] = {
			NOMIC_ENTITY_CHUNK_EXCEPTION_HEADER "",
			};

		#define NOMIC_ENTITY_CHUNK_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NOMIC_ENTITY_CHUNK_EXCEPTION_MAX) ? EXCETION_UNKNOWN : \
				STRING_CHECK(NOMIC_ENTITY_CHUNK_EXCEPTION_STR[_TYPE_]))

		#define THROW_NOMIC_ENTITY_CHUNK_EXCEPTION(_EXCEPT_) \
			THROW_NOMIC_ENTITY_CHUNK_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NOMIC_ENTITY_CHUNK_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NOMIC_ENTITY_CHUNK_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
	}
}

#endif // NOMIC_ENTITY_CHUNK_TYPE_H_
