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

#ifndef NOMIC_EVENT_MANAGER_H_
#define NOMIC_EVENT_MANAGER_H_

#include <map>
#include <queue>
#include <set>
#include "../core/singleton.h"
#include "../core/thread.h"
#include "./queue.h"

namespace nomic {

	namespace event {

		class manager :
				public SINGLETON_CLASS(nomic::event::manager),
				protected nomic::core::thread {

			public:

				~manager(void);

				void flush_events(void);

				void receive_event(
					__in const nomic::core::event &event
					);

				void register_id(
					__in uint32_t id,
					__in nomic::event::queue *handle
					);

				bool registered_id(
					__in uint32_t id,
					__in_opt nomic::event::queue *handle = nullptr
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void unregister_all_ids(
					__in_opt nomic::event::queue *handle = nullptr
					);

				void unregister_id(
					__in uint32_t id,
					__in_opt nomic::event::queue *handle = nullptr
					);

			protected:

				SINGLETON_CLASS_BASE(nomic::event::manager);

				manager(void);

				manager(
					__in const manager &other
					) = delete;

				manager &operator=(
					__in const manager &other
					) = delete;

				std::map<uint32_t, std::set<nomic::event::queue *>>::iterator find_id(
					__in uint32_t id
					);

				bool on_initialize(void);

				bool on_run(void);

				void on_stop(void);

				void on_uninitialize(void);

				bool poll_event(
					__inout nomic::core::event &event
					);

				void send_event(
					__in nomic::core::event &event
					);

				std::map<uint32_t, std::set<nomic::event::queue *>> m_id;

				std::mutex m_mutex;

				std::queue<nomic::core::event> m_queue;
		};
	}
}

#endif // NOMIC_EVENT_MANAGER_H_
