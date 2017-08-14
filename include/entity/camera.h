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

#ifndef NOMIC_ENTITY_CAMERA_H_
#define NOMIC_ENTITY_CAMERA_H_

#include <map>
#include "./object.h"
#include "../event/input.h"
#include "../terrain/manager.h"

namespace nomic {

	namespace entity {

		class camera :
				public nomic::entity::object {

			public:

				explicit camera(
					__in const glm::uvec2 &dimensions,
					__in_opt const glm::vec3 &position = CAMERA_POSITION_DEFAULT,
					__in_opt const glm::vec3 &rotation = CAMERA_ROTATION_DEFAULT,
					__in_opt const glm::vec3 &up = CAMERA_UP_DEFAULT,
					__in_opt float fov = CAMERA_FOV_DEFAULT
					);

				camera(
					__in const camera &other
					);

				virtual ~camera(void);

				camera &operator=(
					__in const camera &other
					);

				glm::uvec3 block(void);

				bool block_changed(void);

				glm::vec3 block_position(void);

				glm::uvec3 block_previous(void);

				glm::ivec2 chunk(void);

				bool chunk_changed(void);

				glm::ivec2 chunk_previous(void);

				glm::uvec2 dimensions(void) const;

				float fov(void) const;

				void key(
					__in uint16_t scancode,
					__in uint16_t modifier,
					__in uint8_t state
					);

				void motion(
					__in uint32_t state,
					__in int32_t x,
					__in int32_t y,
					__in int32_t x_relative,
					__in int32_t y_relative
					);

				void move(
					__in const glm::ivec2 &chunk,
					__in const glm::uvec3 &block
					);

				float pitch(void);

				void set_dimensions(
					__in const glm::uvec2 &dimensions
					);

				void set_fov(
					__in float fov
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void update(void);

				glm::mat4 &view(void);

				void wheel(
					__in uint32_t direction,
					__in int32_t x,
					__in int32_t y
					);

				float yaw(void);

			protected:

				bool determine_block_passable(
					__in uint8_t type
					);

				void update_perspective(void);

				glm::uvec2 m_dimensions;

				bool m_falling;

				float m_fov;

				uint8_t m_jump_timeout;

				std::map<uint16_t, std::pair<uint16_t, bool>> m_key;

				glm::vec2 m_motion;

				glm::uvec3 m_position_block;

				glm::uvec3 m_position_block_previous;

				glm::ivec2 m_position_chunk;

				glm::ivec2 m_position_chunk_previous;

				glm::vec2 m_rotation_previous;

				bool m_sprinting;

				glm::vec3 m_velocity;

				int32_t m_wheel;
		};
	}
}

#endif // NOMIC_ENTITY_CAMERA_H_
