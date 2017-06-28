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

#ifndef NOMIC_CORE_UNIFORM_H_
#define NOMIC_CORE_UNIFORM_H_

#include "./transform.h"

namespace nomic {

	namespace core {

		class uniform {

			public:

				explicit uniform(
					__in_opt const glm::mat4 &model = UNIFORM_MATRIX_DEFAULT,
					__in_opt const glm::mat4 &projection = UNIFORM_MATRIX_DEFAULT,
					__in_opt const glm::mat4 &view = UNIFORM_MATRIX_DEFAULT
					);

				uniform(
					__in const uniform &other
					);

				virtual ~uniform(void);

				uniform &operator=(
					__in const uniform &other
					);

				glm::mat4 &model(void);

				glm::mat4 &model(
					__in const glm::vec3 &position,
					__in const glm::vec3 &rotation
					);

				glm::mat4 &projection(void);

				void reset(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				glm::mat4 &view(void);

			protected:

				glm::mat4 m_model;

				glm::mat4 m_projection;

				glm::mat4 m_view;
		};
	}
}

#endif // NOMIC_CORE_UNIFORM_H_
