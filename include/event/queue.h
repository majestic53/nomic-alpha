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

#ifndef NOMIC_EVENT_QUEUE_H_
#define NOMIC_EVENT_QUEUE_H_

#include <queue>
#include <set>
#include "../core/event.h"

namespace nomic {

	namespace event {

		class queue {

			public:

				queue(void);

				virtual ~queue(void);

				void flush_events(void);

				bool poll_event(
					__inout nomic::core::event &event
					);

				void receive_event(
					__in const nomic::core::event &event
					);

				void register_for_event(
					__in uint32_t id
					);

				bool registered_for_event(
					__in uint32_t id
					);

				void send_event(
					__in const nomic::core::event &event
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void unregister_all_events(void);

				void unregister_event(
					__in uint32_t id
					);

			protected:

				void unregister_event_id(
					__in uint32_t id
					);

				std::set<uint32_t> m_id;

				std::mutex m_mutex;

				std::queue<nomic::core::event> m_queue;
		};
	}
}

#endif // NOMIC_EVENT_QUEUE_H_
