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

#ifndef NOMIC_ENTITY_MANAGER_H_
#define NOMIC_ENTITY_MANAGER_H_

#include <map>
#include <set>
#include "../core/entity.h"
#include "../core/singleton.h"

namespace nomic {

	namespace entity {

		class manager :
				public SINGLETON_CLASS(nomic::entity::manager) {

			public:

				~manager(void);

				bool contains(
					__in uint32_t id
					);

				void add(
					__in nomic::core::entity *handle
					);

				void remove(
					__in nomic::core::entity *handle
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void update(void);

			protected:

				SINGLETON_CLASS_BASE(nomic::entity::manager);

				manager(void);

				manager(
					__in const manager &other
					) = delete;

				manager &operator=(
					__in const manager &other
					) = delete;

				std::set<nomic::core::entity *>::iterator find_handle(
					__in nomic::core::entity *handle,
					__in std::map<uint32_t, std::set<nomic::core::entity *>>::iterator iter
					);

				std::map<uint32_t, std::set<nomic::core::entity *>>::iterator find_id(
					__in uint32_t id
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				std::map<uint32_t, std::set<nomic::core::entity *>> m_id;
		};
	}
}

#endif // NOMIC_ENTITY_MANAGER_H_
