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

#ifndef NOMIC_ENTITY_SELECTOR_H_
#define NOMIC_ENTITY_SELECTOR_H_

#include "./object.h"

namespace nomic {

	namespace entity {

		class selector :
				public nomic::entity::object {

			public:

				selector(
					__in_opt const glm::vec4 &color = SELECTOR_COLOR_DEFAULT,
					__in_opt float scale = SELECTOR_SCALE_DEFAULT,
					__in_opt const glm::vec3 &position = TRANSFORM_POSITION_DEFAULT,
					__in_opt const glm::vec3 &rotation = TRANSFORM_ROTATION_DEFAULT,
					__in_opt const glm::vec3 &up = TRANSFORM_UP_DEFAULT
					);

				selector(
					__in const selector &other
					);

				virtual ~selector(void);

				selector &operator=(
					__in const selector &other
					);

				virtual void on_render(
					__in nomic::core::renderer &renderer,
					__in void *textures,
					__in float delta
					);

				void set_color(
					__in const glm::vec4 &color
					);

				void set_face(
					__in uint8_t face
					);

				void set_scale(
					__in float scale
					);

				void show_bounds(
					__in bool show
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				void reconfigure(void);

				void setup(void);

				glm::vec4 m_color;

				uint8_t m_face;

				float m_scale;

				bool m_show_bounds;
		};
	}
}

#endif // NOMIC_ENTITY_SELECTOR_H_
