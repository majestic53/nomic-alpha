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

#ifndef NOMIC_CORE_ENTITY_H_
#define NOMIC_CORE_ENTITY_H_

#include "./id.h"
#include "./object.h"
#include "./transform.h"

namespace nomic {

	namespace core {

		class entity :
				public nomic::core::id,
				public nomic::core::object,
				public nomic::core::transform {

			public:

				explicit entity(
					__in uint32_t type,
					__in uint32_t subtype = SUBTYPE_UNDEFINED,
					__in_opt const glm::vec3 &position = TRANSFORM_POSITION_DEFAULT,
					__in_opt const glm::vec3 &rotation = TRANSFORM_ROTATION_DEFAULT,
					__in_opt const glm::vec3 &up = TRANSFORM_UP_DEFAULT
					);

				entity(
					__in const entity &other
					);

				virtual ~entity(void);

				entity &operator=(
					__in const entity &other
					);

				void enable(
					__in bool state
					);

				bool enabled(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				virtual void update(void);

			protected:

				void add(void);

				void remove(void);

				bool m_enabled;
		};
	}
}

#endif // NOMIC_CORE_ENTITY_H_
