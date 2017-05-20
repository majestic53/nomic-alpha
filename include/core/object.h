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

#ifndef NOMIC_CORE_OBJECT_H_
#define NOMIC_CORE_OBJECT_H_

#include "../define.h"

namespace nomic {

	namespace core {

		class object {

			public:

				object(
					__in uint32_t type,
					__in_opt uint32_t subtype = SUBTYPE_UNDEFINED
					);

				object(
					__in const object &other
					);

				virtual ~object(void);

				object &operator=(
					__in const object &other
					);

				bool has_subtype(void) const;

				uint32_t subtype(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				uint32_t type(void) const;

			protected:

				uint32_t m_subtype;

				uint32_t m_type;
		};
	}
}

#endif // NOMIC_CORE_OBJECT_H_
