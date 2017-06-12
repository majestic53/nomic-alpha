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

#ifndef NOMIC_ENTITY_PLAIN_H_
#define NOMIC_ENTITY_PLAIN_H_

#include "../graphic/texture.h"
#include "./object.h"

namespace nomic {

	namespace entity {

		class plain :
				public nomic::entity::object,
				protected nomic::graphic::texture {

			public:

				explicit plain(
					__in_opt const std::string &path = std::string(),
					__in_opt const glm::vec2 dimension = PLAIN_DIMENSION_DEFAULT,
					__in_opt float scale = PLAIN_SCALE_DEFAULT,
					__in_opt const glm::vec3 &position = TRANSFORM_POSITION_DEFAULT,
					__in_opt const glm::vec3 &rotation = TRANSFORM_ROTATION_DEFAULT,
					__in_opt const glm::vec3 &up = TRANSFORM_UP_DEFAULT
					);

				explicit plain(
					__in const glm::vec4 &color,
					__in_opt const glm::vec2 dimension = PLAIN_DIMENSION_DEFAULT,
					__in_opt float scale = PLAIN_SCALE_DEFAULT,
					__in_opt const glm::vec3 &position = TRANSFORM_POSITION_DEFAULT,
					__in_opt const glm::vec3 &rotation = TRANSFORM_ROTATION_DEFAULT,
					__in_opt const glm::vec3 &up = TRANSFORM_UP_DEFAULT
					);

				plain(
					__in const plain &other
					);

				virtual ~plain(void);

				plain &operator=(
					__in const plain &other
					);

				void clear_texture(void);

				glm::vec2 dimensions(void) const;

				virtual void on_render(
					__in float delta
					);

				virtual void on_update(void);

				void set_color(
					__in const glm::vec4 &color
					);

				void set_dimensions(
					__in const glm::vec2 dimension,
					__in float scale
					);

				void set_position(
					__in const glm::vec3 &position,
					__in_opt const glm::vec3 &rotation = TRANSFORM_ROTATION_DEFAULT,
					__in_opt const glm::vec3 &up = TRANSFORM_UP_DEFAULT
					);

				void set_texture(
					__in const std::string &path
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				void reconfigure(void);

				glm::vec4 m_color;

				glm::vec2 m_dimension;

				bool m_texture_set;
		};
	}
}

#endif // NOMIC_ENTITY_PLAIN_H_
