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

#ifndef NOMIC_TERRAIN_CHUNK_H_
#define NOMIC_TERRAIN_CHUNK_H_

#include <queue>
#include <tuple>
#include <vector>
#include "../define.h"

namespace nomic {

	namespace terrain {

		class chunk {

			public:

				explicit chunk(
					__in_opt const glm::ivec2 &position = glm::ivec2()
					);

				chunk(
					__in const chunk &other
					);

				virtual ~chunk(void);

				chunk &operator=(
					__in const chunk &other
					);

				uint8_t attributes(
					__in const glm::uvec3 &position
					) const;

				uint8_t height(
					__in const glm::uvec2 &position
					) const;

				glm::ivec2 position(void) const;

				void set(
					__in const glm::uvec3 &position,
					__in uint8_t type,
					__in_opt uint8_t attributes = BLOCK_ATTRIBUTES_DEFAULT
					);

				uint8_t set_block(
					__in const glm::uvec3 &position,
					__in uint8_t type,
					__in_opt uint8_t attributes = BLOCK_ATTRIBUTES_DEFAULT
					);

				void set_spawn(
					__in const glm::uvec3 &position,
					__in uint8_t type,
					__in_opt uint32_t timeout = SPAWN_TIMEOUT_DEFAULT
					);

				void set_position(
					__in const glm::ivec2 &position
					);

				std::vector<std::pair<glm::uvec3, uint8_t>> spawn(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				uint8_t type(
					__in const glm::uvec3 &position
					) const;

				void update(void);

			protected:

				void clear(void);

				void copy(
					__in const chunk &other
					);

				bool m_active = false;

				uint8_t m_block[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];

				uint8_t m_block_attributes[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_WIDTH];

				uint8_t m_height[CHUNK_WIDTH][CHUNK_WIDTH];

				glm::ivec2 m_position;

				std::queue<std::tuple<glm::uvec3, uint8_t, uint8_t>> m_queue;

				std::vector<std::tuple<glm::uvec3, uint32_t, uint8_t>> m_spawn;
		};
	}
}

#endif // NOMIC_TERRAIN_CHUNK_H_
