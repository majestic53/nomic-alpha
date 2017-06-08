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

#ifndef NOMIC_RENDER_MANAGER_TYPE_H_
#define NOMIC_RENDER_MANAGER_TYPE_H_

#include "../../include/exception.h"

namespace nomic {

	namespace render {

		#define NOMIC_RENDER_MANAGER_HEADER "[NOMIC::RENDER::MANAGER]"
#ifndef NDEBUG
		#define NOMIC_RENDER_MANAGER_EXCEPTION_HEADER NOMIC_RENDER_MANAGER_HEADER " "
#else
		#define NOMIC_RENDER_MANAGER_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NOMIC_RENDER_MANAGER_EXCEPTION_ENTITY_HANDLE_DUPLICATE = 0,
			NOMIC_RENDER_MANAGER_EXCEPTION_ENTITY_HANDLE_INVALID,
			NOMIC_RENDER_MANAGER_EXCEPTION_HANDLE_DUPLICATE,
			NOMIC_RENDER_MANAGER_EXCEPTION_HANDLE_INVALID,
			NOMIC_RENDER_MANAGER_EXCEPTION_HANDLE_NOT_FOUND,
			NOMIC_RENDER_MANAGER_EXCEPTION_ID_INVALID,
			NOMIC_RENDER_MANAGER_EXCEPTION_ID_NOT_FOUND,
			NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED,
		};

		#define NOMIC_RENDER_MANAGER_EXCEPTION_MAX NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED

		static const std::string NOMIC_RENDER_MANAGER_EXCEPTION_STR[] = {
			NOMIC_RENDER_MANAGER_EXCEPTION_HEADER "Duplicate entity handle",
			NOMIC_RENDER_MANAGER_EXCEPTION_HEADER "Invalid entity handle",
			NOMIC_RENDER_MANAGER_EXCEPTION_HEADER "Duplicate renderer handle",
			NOMIC_RENDER_MANAGER_EXCEPTION_HEADER "Invalid renderer handle",
			NOMIC_RENDER_MANAGER_EXCEPTION_HEADER "Renderer handle does not exist",
			NOMIC_RENDER_MANAGER_EXCEPTION_HEADER "Invalid renderer id",
			NOMIC_RENDER_MANAGER_EXCEPTION_HEADER "Renderer id does not exist",
			NOMIC_RENDER_MANAGER_EXCEPTION_HEADER "Renderer manager is uninitialized",
			};

		#define NOMIC_RENDER_MANAGER_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NOMIC_RENDER_MANAGER_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NOMIC_RENDER_MANAGER_EXCEPTION_STR[_TYPE_]))

		#define THROW_NOMIC_RENDER_MANAGER_EXCEPTION(_EXCEPT_) \
			THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_STRING(_EXCEPT_), _FORMAT_, __VA_ARGS__)
	}
}

#endif // NOMIC_RENDER_MANAGER_TYPE_H_