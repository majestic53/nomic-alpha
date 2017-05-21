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

#ifndef NOMIC_UUID_MANAGER_H_
#define NOMIC_UUID_MANAGER_H_

#include <map>
#include <set>
#include "../core/singleton.h"

namespace nomic {

	namespace uuid {

		class manager :
				public SINGLETON_CLASS(nomic::uuid::manager) {

			public:

				~manager(void);

				bool contains(
					__in uint32_t id
					);

				size_t decrement(
					__in uint32_t id
					);

				uint32_t generate(void);

				size_t increment(
					__in uint32_t id
					);

				size_t references(
					__in uint32_t id
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				SINGLETON_CLASS_BASE(nomic::uuid::manager);

				manager(void);

				manager(
					__in const manager &other
					) = delete;

				manager &operator=(
					__in const manager &other
					) = delete;

				void clear(void);

				std::map<uint32_t, size_t>::iterator find(
					__in uint32_t id
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				std::map<uint32_t, size_t> m_id;

				uint32_t m_id_next;

				std::set<uint32_t> m_id_surplus;

				std::mutex m_mutex;
		};
	}
}

#endif // NOMIC_UUID_MANAGER_H_
