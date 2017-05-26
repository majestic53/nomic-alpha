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

#include "../../include/event/queue.h"
#include "../../include/event/manager.h"
#include "../../include/trace.h"
#include "./queue_type.h"

namespace nomic {

	namespace event {

		queue::queue(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		queue::~queue(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			unregister_all_events();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		queue::flush_events(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			std::lock_guard<std::mutex> lock(m_mutex);

			while(!m_queue.empty()) {
				m_queue.pop();
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		queue::poll_event(
			__inout nomic::core::event &event
			)
		{
			bool result;

			TRACE_ENTRY(LEVEL_VERBOSE);

			std::lock_guard<std::mutex> lock(m_mutex);

			result = !m_queue.empty();
			if(result) {
				event = m_queue.front();
				m_queue.pop();
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		queue::receive_event(
			__in const nomic::core::event &event
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			std::lock_guard<std::mutex> lock(m_mutex);

			while(m_queue.size() >= QUEUE_MAX) {
				m_queue.pop();
			}

			m_queue.push(event);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		queue::register_for_event(
			__in uint32_t id
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,"Id=%x", id);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(m_id.find(id) != m_id.end()) {
				THROW_NOMIC_EVENT_QUEUE_EXCEPTION_FORMAT(NOMIC_EVENT_QUEUE_EXCEPTION_DUPLICATE, "Id=%x", id);
			}

			nomic::event::manager &instance = nomic::event::manager::acquire();
			if(instance.initialized()) {
				instance.register_id(id, this);
			}

			instance.release();
			m_id.insert(id);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		queue::registered_for_event(
			__in uint32_t id
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,"Id=%x", id);

			std::lock_guard<std::mutex> lock(m_mutex);

			result = (m_id.find(id) != m_id.end());

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		std::string 
		queue::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_EVENT_QUEUE_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Id[" << m_id.size() << "]";

				if(!m_id.empty()) {
					result << "={";

					for(std::set<uint32_t>::iterator iter = m_id.begin(); iter != m_id.end(); ++iter) {

						if(iter != m_id.begin()) {
							result << ", ";
						}

						result << SCALAR_AS_HEX(uint32_t, *iter);
					}

					result << "}";
				}

				result << ", Queue[" << m_queue.size() << "]";
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		queue::unregister_all_events(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			std::lock_guard<std::mutex> lock(m_mutex);

			for(std::set<uint32_t>::iterator iter = m_id.begin(); iter != m_id.end(); ++iter) {
				unregister_event_id(*iter);
			}

			m_id.clear();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		queue::unregister_event(
			__in uint32_t id
			)
		{
			std::set<uint32_t>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,"Id=%x", id);

			std::lock_guard<std::mutex> lock(m_mutex);

			result = m_id.find(id);
			if(result == m_id.end()) {
				THROW_NOMIC_EVENT_QUEUE_EXCEPTION_FORMAT(NOMIC_EVENT_QUEUE_EXCEPTION_NOT_FOUND, "Id=%x", id);
			}

			unregister_event_id(*result);
			m_id.erase(result);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		queue::unregister_event_id(
			__in uint32_t id
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,"Id=%x", id);

			nomic::event::manager &instance = nomic::event::manager::acquire();
			if(instance.initialized()) {
				instance.unregister_id(id, this);
			}

			instance.release();

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
