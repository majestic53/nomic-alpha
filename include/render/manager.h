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

namespace nomic {

	namespace render {

		class manager :
				public SINGLETON_CLASS(nomic::render::manager) {

			public:

				~manager(void);

				void add(
					__in nomic::core::renderer *handle
					);

				bool contains(
					__in GLuint id
					);

				bool contains_registration(
					__in nomic::core::entity *handle,
					__in GLuint id
					);

				void remove(
					__in nomic::core::renderer *handle
					);

				void register_entity(
					__in nomic::core::entity *handle,
					__in GLuint id
					);

				void render(
					__in glm::mat4 &projection,
					__in glm::mat4 &view,
					__in float delta
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void unregister_all_entities(
					__in GLuint id
					);

				void unregister_entity(
					__in nomic::core::entity *handle,
					__in GLuint id
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

				std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>::iterator find_handle(
					__in nomic::core::renderer *handle
					);

				std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator find_id(
					__in GLuint id
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>> m_id;
		};
	}
}

#endif // NOMIC_RENDER_MANAGER_H_
