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

#ifndef NOMIC_RENDER_MANAGER_H_
#define NOMIC_RENDER_MANAGER_H_

#include <map>
#include <set>
#include "../core/entity.h"
#include "../core/renderer.h"
#include "../core/singleton.h"
#include "../graphic/atlas.h"

namespace nomic {

	namespace render {

		class manager :
				public SINGLETON_CLASS(nomic::render::manager) {

			public:

				~manager(void);

				void add(
					__in nomic::core::renderer *renderer
					);

				bool contains(
					__in uint32_t type
					);

				bool contains_entity(
					__in nomic::core::entity *entity,
					__in uint32_t type
					);

				void remove(
					__in nomic::core::renderer *renderer
					);

				void register_entity(
					__in nomic::core::entity *entity,
					__in uint32_t type
					);

				void render(
					__in const glm::vec3 &position,
					__in const glm::vec3 &rotation,
					__in const glm::mat4 &projection,
					__in const glm::mat4 &view,
					__in const glm::uvec2 &view_dimensions,
					__in nomic::graphic::atlas &textures,
					__in float cycle,
					__in float delta,
					__in const glm::vec4 &ambient,
					__in const glm::vec4 &ambient_background,
					__in const glm::vec3 &ambient_position,
					__in bool underwater
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void unregister_all_entities(
					__in uint32_t type
					);

				void unregister_entity(
					__in nomic::core::entity *entity,
					__in uint32_t type
					);

			protected:

				SINGLETON_CLASS_BASE(nomic::render::manager);

				manager(void);

				manager(
					__in const manager &other
					) = delete;

				manager &operator=(
					__in const manager &other
					) = delete;

				std::map<uint32_t, std::pair<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator find(
					__in uint32_t type
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				std::map<uint32_t, std::pair<nomic::core::renderer *, std::set<nomic::core::entity *>>> m_entry;

				std::mutex m_mutex;
		};
	}
}

#endif // NOMIC_RENDER_MANAGER_H_
