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

#ifndef NOMIC_ENTITY_BLOCK_H_
#define NOMIC_ENTITY_BLOCK_H_

#include <map>
#include "./object.h"
#include "../graphic/texture.h"

namespace nomic {

	namespace entity {

		class block :
				public nomic::entity::object {

			public:

				block(
					__in_opt const std::map<uint32_t, std::string> &paths = std::map<uint32_t, std::string>(),
					__in_opt float scale = BLOCK_SCALE_DEFAULT,
					__in_opt GLenum wrap_s = BLOCK_WRAP_S_DEFAULT,
					__in_opt GLenum wrap_t = BLOCK_WRAP_T_DEFAULT,
					__in_opt GLenum filter_min = BLOCK_FILTER_MIN_DEFAULT,
					__in_opt GLenum filter_mag = BLOCK_FILTER_MAG_DEFAULT,
					__in_opt const glm::vec3 &position = TRANSFORM_POSITION_DEFAULT,
					__in_opt const glm::vec3 &rotation = TRANSFORM_ROTATION_DEFAULT,
					__in_opt const glm::vec3 &up = TRANSFORM_UP_DEFAULT
					);

				block(
					__in const block &other
					);

				virtual ~block(void);

				block &operator=(
					__in const block &other
					);

				virtual void on_render(
					__in nomic::core::renderer &renderer,
					__in void *textures,
					__in float delta
					);

				void set_scale(
					__in float scale
					);

				void set_textures(
					__in_opt const std::map<uint32_t, std::string> &paths,
					__in_opt GLenum wrap_s = BLOCK_WRAP_S_DEFAULT,
					__in_opt GLenum wrap_t = BLOCK_WRAP_T_DEFAULT,
					__in_opt GLenum filter_min = BLOCK_FILTER_MIN_DEFAULT,
					__in_opt GLenum filter_mag = BLOCK_FILTER_MAG_DEFAULT
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				void reconfigure(void);

				std::vector<nomic::graphic::texture> m_face;

				float m_scale;
		};
	}
}

#endif // NOMIC_ENTITY_BLOCK_H_
