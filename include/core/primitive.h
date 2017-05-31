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

#ifndef NOMIC_CORE_PRIMITIVE_H_
#define NOMIC_CORE_PRIMITIVE_H_

#include "./object.h"

namespace nomic {

	namespace core {

		class primitive :
				public nomic::core::object {

			public:

				primitive(
					__in uint32_t type,
					__in GLenum subtype = SUBTYPE_UNDEFINED
					);

				primitive(
					__in const primitive &other
					);

				virtual ~primitive(void);

				primitive &operator=(
					__in const primitive &other
					);

				GLuint handle(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				void decrement(void);

				void generate(void);

				void increment(void);

				GLuint m_handle;
		};
	}
}

#endif // NOMIC_CORE_PRIMITIVE_H_
