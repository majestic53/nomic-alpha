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

#ifndef NOMIC_TERRAIN_MANAGER_H_
#define NOMIC_TERRAIN_MANAGER_H_

#include <map>
#include "../core/renderer.h"
#include "../core/singleton.h"
#include "../entity/chunk.h"
#include "./generator.h"

namespace nomic {

	namespace terrain {

		class manager :
				public SINGLETON_CLASS(nomic::terrain::manager) {

			public:

				~manager(void);

				nomic::entity::chunk *at(
					__in const glm::ivec2 &position,
					__in_opt bool generate = true
					);

				std::map<std::pair<int32_t, int32_t>, nomic::entity::chunk *> &chunks(void);

				bool contains(
					__in const glm::ivec2 &position
					);

				nomic::terrain::generator &generator(void);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				SINGLETON_CLASS_BASE(nomic::terrain::manager);

				manager(void);

				manager(
					__in const manager &other
					) = delete;

				manager &operator=(
					__in const manager &other
					) = delete;

				std::map<std::pair<int32_t, int32_t>, nomic::entity::chunk *>::iterator find(
					__in const glm::ivec2 &position
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				std::map<std::pair<int32_t, int32_t>, nomic::entity::chunk *> m_chunk;

				nomic::terrain::generator m_generator;
		};
	}
}

#endif // NOMIC_TERRAIN_MANAGER_H_
