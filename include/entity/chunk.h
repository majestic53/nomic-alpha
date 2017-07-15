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

#ifndef NOMIC_ENTITY_CHUNK_H_
#define NOMIC_ENTITY_CHUNK_H_

#include <map>
#include <tuple>
#include "../terrain/chunk.h"
#include "../terrain/generator.h"
#include "./object.h"

namespace nomic {

	namespace entity {

		typedef std::tuple<std::vector<glm::vec4>, std::vector<glm::vec2>, std::vector<glm::vec3>, uint32_t, uint32_t> chunk_data;

		class chunk :
				public nomic::entity::object,
				protected nomic::terrain::chunk {

			public:

				chunk(
					__in const glm::ivec2 &position,
					__in nomic::terrain::generator &generator
					);

				chunk(
					__in const chunk &other
					);

				virtual ~chunk(void);

				chunk &operator=(
					__in const chunk &other
					);

				uint8_t block_attributes(
					__in const glm::uvec3 &position
					) const;

				uint8_t block_height(
					__in const glm::uvec2 &position
					) const;

				uint8_t block_type(
					__in const glm::uvec3 &position
					) const;

				glm::ivec2 position(void) const;

				virtual void on_render(
					__in nomic::core::renderer &renderer,
					__in void *textures,
					__in float delta
					);

				virtual void on_update(
					__in void *runtime,
					__in void *camera
					);

				void rebuild(
					__in_opt nomic::entity::chunk *right = nullptr,
					__in_opt nomic::entity::chunk *left = nullptr,
					__in_opt nomic::entity::chunk *back = nullptr,
					__in_opt nomic::entity::chunk *front = nullptr
					);

				uint8_t set_block(
					__in const glm::uvec3 &position,
					__in uint8_t type,
					__in_opt uint8_t attributes = BLOCK_ATTRIBUTES_DEFAULT
					);

				void teardown(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void update(
					__in_opt nomic::entity::chunk *right,
					__in_opt nomic::entity::chunk *left,
					__in_opt nomic::entity::chunk *back,
					__in_opt nomic::entity::chunk *front
					);

			protected:

				void add_face(
					__in const glm::vec3 &position,
					__in uint32_t face,
					__in std::map<uint8_t, chunk_data>::iterator entry
					);

				std::map<uint8_t, chunk_data>::iterator 
				add_face_type(
					__in uint8_t type,
					__in uint32_t face
					);

				bool m_changed;

				nomic::entity::chunk *m_chunk_back;

				nomic::entity::chunk *m_chunk_front;

				nomic::entity::chunk *m_chunk_left;

				nomic::entity::chunk *m_chunk_right;

				std::map<uint8_t, chunk_data> m_face;
		};
	}
}

#endif // NOMIC_ENTITY_CHUNK_H_
