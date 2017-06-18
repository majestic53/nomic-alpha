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

#ifndef NOMIC_ENTITY_OBJECT_H_
#define NOMIC_ENTITY_OBJECT_H_

#include "../core/entity.h"
#include "../core/uniform.h"
#include "../graphic/vao.h"

namespace nomic {

	namespace entity {

		class object :
				public nomic::core::entity,
				public nomic::core::uniform {

			public:

				object(
					__in uint32_t type,
					__in uint32_t subtype = SUBTYPE_UNDEFINED,
					__in_opt const glm::vec3 &position = TRANSFORM_POSITION_DEFAULT,
					__in_opt const glm::vec3 &rotation = TRANSFORM_ROTATION_DEFAULT,
					__in_opt const glm::vec3 &up = TRANSFORM_UP_DEFAULT,
					__in_opt const glm::mat4 &model = UNIFORM_MATRIX_DEFAULT,
					__in_opt const glm::mat4 &projection = UNIFORM_MATRIX_DEFAULT,
					__in_opt const glm::mat4 &view = UNIFORM_MATRIX_DEFAULT
					);

				object(
					__in const object &other
					);

				virtual ~object(void);

				object &operator=(
					__in const object &other
					);

				virtual void on_render(
					__in nomic::core::renderer &renderer,
					__in float delta
					);

				virtual void on_update(
					__in void *runtime,
					__in void *camera
					);

				virtual void on_view_change(void);

				void set_view_dimensions(
					__in const glm::vec2 &view_dimensions
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				nomic::graphic::vao &vertex_array(void);

				glm::uvec2 view_dimensions(void) const;

				uint32_t view_height(void) const;

				uint32_t view_width(void) const;

			protected:

				nomic::graphic::vao m_vao;

				glm::uvec2 m_view_dimensions;
		};
	}
}

#endif // NOMIC_ENTITY_OBJECT_H_
