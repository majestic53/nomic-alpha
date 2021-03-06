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

#ifndef NOMIC_ENTITY_DIAGNOSTIC_H_
#define NOMIC_ENTITY_DIAGNOSTIC_H_

#include "./string.h"

namespace nomic {

	namespace entity {

		class diagnostic :
				public nomic::entity::string {

			public:

				diagnostic(
					__in_opt bool verbose = true,
					__in_opt const glm::vec4 &color = STRING_COLOR_DEFAULT,
					__in_opt float scale = STRING_SCALE_DEFAULT
					);

				diagnostic(
					__in const diagnostic &other
					);

				virtual ~diagnostic(void);

				diagnostic &operator=(
					__in const diagnostic &other
					);

				void on_update(
					__in void *runtime,
					__in void *camera
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				bool m_verbose;
		};
	}
}

#endif // NOMIC_ENTITY_DIAGNOSTIC_H_
