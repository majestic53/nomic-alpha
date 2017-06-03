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

#ifndef NOMIC_GRAPHIC_CAMERA_H_
#define NOMIC_GRAPHIC_CAMERA_H_

#include <map>
#include "../core/entity.h"
#include "../core/uniform.h"
#include "../event/input.h"

namespace nomic {

	namespace graphic {

		class camera :
				public nomic::core::entity,
				public nomic::core::uniform,
				protected nomic::event::input {

			public:

				explicit camera(
					__in const glm::uvec2 &dimension,
					__in_opt const glm::vec3 &position = TRANSFORM_POSITION_DEFAULT,
					__in_opt const glm::vec3 &rotation = TRANSFORM_ROTATION_DEFAULT,
					__in_opt const glm::vec3 &up = TRANSFORM_UP_DEFAULT,
					__in_opt float fov = CAMERA_FOV_DEFAULT
					);

				camera(
					__in const camera &other
					);

				virtual ~camera(void);

				camera &operator=(
					__in const camera &other
					);

				void render(
					__in float delta
					);

				void set_dimensions(
					__in const glm::uvec2 &dimension
					);

				void set_fov(
					__in float fov
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void update_perspective(void);

			protected:

				void on_key(
					__in uint16_t scancode,
					__in uint16_t modifier,
					__in uint8_t state
					);

				void on_motion(
					__in uint32_t state,
					__in int32_t x,
					__in int32_t y,
					__in int32_t x_relative,
					__in int32_t y_relative
					);

				void on_wheel(
					__in uint32_t direction,
					__in int32_t x,
					__in int32_t y
					);

				glm::uvec2 m_dimension;

				float m_fov;

				std::map<std::pair<uint16_t, uint16_t>, bool> m_key;

				glm::vec2 m_motion;

				glm::vec2 m_rotation_previous;

				int32_t m_wheel;
		};
	}
}

#endif // NOMIC_GRAPHIC_CAMERA_H_