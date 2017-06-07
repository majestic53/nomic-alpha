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

#ifndef NOMIC_GRAPHIC_MANAGER_H_
#define NOMIC_GRAPHIC_MANAGER_H_

#include <map>
#include "../core/singleton.h"

namespace nomic {

	namespace graphic {

		class manager :
				public SINGLETON_CLASS(nomic::graphic::manager) {

			public:

				~manager(void);

				bool contains(
					__in uint32_t type,
					__in GLuint handle
					);

				size_t decrement(
					__in uint32_t type,
					__in GLuint handle
					);

				GLuint generate(
					__in uint32_t type,
					__in_opt GLenum subtype = SUBTYPE_UNDEFINED
					);

				size_t increment(
					__in uint32_t type,
					__in GLuint handle
					);

				size_t references(
					__in uint32_t type,
					__in GLuint handle
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				SINGLETON_CLASS_BASE(nomic::graphic::manager);

				manager(void);

				manager(
					__in const manager &other
					) = delete;

				manager &operator=(
					__in const manager &other
					) = delete;

				void destroy(
					__in uint32_t type,
					__in std::map<GLuint, std::pair<size_t, GLenum>>::iterator iter
					);

				std::map<GLuint, std::pair<size_t, GLenum>>::iterator find(
					__in uint32_t type,
					__in GLuint handle
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				std::map<uint32_t, std::map<GLuint, std::pair<size_t, GLenum>>> m_handle;

				std::mutex m_mutex;
		};
	}
}

#endif // NOMIC_GRAPHIC_MANAGER_H_
