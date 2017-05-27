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

#ifndef NOMIC_CORE_TRANSFORM_H_
#define NOMIC_CORE_TRANSFORM_H_

#include "../define.h"

namespace nomic {

	namespace core {

		class transform {

			public:

				explicit transform(
					__in_opt const glm::vec3 &position = TRANSFORM_POSITION_DEFAULT,
					__in_opt const glm::vec3 &rotation = TRANSFORM_ROTATION_DEFAULT,
					__in_opt const glm::vec3 &up = TRANSFORM_UP_DEFAULT
					);

				transform(
					__in const transform &other
					);

				virtual ~transform(void);

				transform &operator=(
					__in const transform &other
					);

				glm::vec3 &position(void);

				void reset(void);

				glm::vec3 &rotation(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				glm::vec3 &up(void);

			protected:

				glm::vec3 m_position;

				glm::vec3 m_rotation;

				glm::vec3 m_up;
		};
	}
}

#endif // NOMIC_CORE_TRANSFORM_H_
