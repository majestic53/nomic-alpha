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

#ifndef NOMIC_ENTITY_SHADOWMAP_H_
#define NOMIC_ENTITY_SHADOWMAP_H_

#include "./object.h"
#include "../graphic/fbo.h"

namespace nomic {

	namespace entity {

		class shadowmap :
				public nomic::entity::object {

			public:

				shadowmap(
					__in_opt GLint depth = SHADOWMAP_DEPTH_DEFAULT,
					__in_opt const glm::uvec2 &dimensions = SHADOWMAP_DIMENSIONS_DEFAULT
					);

				shadowmap(
					__in const shadowmap &other
					);

				virtual ~shadowmap(void);

				shadowmap &operator=(
					__in const shadowmap &other
					);

				nomic::graphic::fbo &fbo(void);

				virtual void on_render(
					__in nomic::core::renderer &renderer,
					__in void *textures,
					__in float delta
					);

				virtual void on_update(
					__in void *runtime,
					__in void *camera
					);

				void setup(
					__in GLint depth,
					__in const glm::uvec2 &dimensions
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				glm::mat4 m_depth;

				nomic::graphic::fbo m_fbo;
		};
	}
}

#endif // NOMIC_ENTITY_SHADOWMAP_H_
