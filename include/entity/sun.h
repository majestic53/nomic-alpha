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

#ifndef NOMIC_ENTITY_SUN_H_
#define NOMIC_ENTITY_SUN_H_

#include "../graphic/texture.h"
#include "./object.h"

namespace nomic {

	namespace entity {

		class sun :
				public nomic::entity::object,
				protected nomic::graphic::texture {

			public:

				sun(
					__in_opt float delta = SUN_DELTA_DEFAULT,
					__in_opt bool cycle = SUN_CYCLE_DEFAULT
					);

				sun(
					__in const sun &other
					);

				virtual ~sun(void);

				sun &operator=(
					__in const sun &other
					);

				virtual void on_render(
					__in nomic::core::renderer &renderer,
					__in void *textures,
					__in float delta
					);

				virtual void on_update(
					__in void *runtime,
					__in void *camera
					);

				glm::vec4 color(void);

				bool cycling(void) const;

				float delta(void) const;

				void set_cycle(
					__in bool cycle
					);

				void set_delta(
					__in float delta
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				void setup(void);

				glm::vec4 m_color;

				bool m_cycle;

				float m_delta;

				float m_radius;
		};
	}
}

#endif // NOMIC_ENTITY_SUN_H_
