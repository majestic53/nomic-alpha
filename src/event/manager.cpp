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

#include "../../include/event/manager.h"
#include "../../include/trace.h"
#include "./manager_type.h"

namespace nomic {

	namespace event {

		manager::manager(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		manager::~manager(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::map<uint32_t, std::set<nomic::event::queue *>>::iterator 
		manager::find_id(
			__in uint32_t id
			)
		{
			std::map<uint32_t, std::set<nomic::event::queue *>>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			result = m_id.find(id);
			if(result == m_id.end()) {
				THROW_NOMIC_EVENT_MANAGER_EXCEPTION_FORMAT(NOMIC_EVENT_MANAGER_EXCEPTION_ID_NOT_FOUND, "Id=%x", id);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		void 
		manager::flush_events(void)
		{
			nomic::core::event event;

			TRACE_ENTRY(LEVEL_VERBOSE);

			while(poll_event(event)) {

				if(nomic::core::thread::active()) {
					send_event(event);
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Event manager initializing...");

			nomic::core::thread::start(true);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Event manager initialized");

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		manager::on_run(void)
		{
			bool result = true;
			nomic::core::event event;

			TRACE_ENTRY(LEVEL_VERBOSE);

			while(poll_event(event)) {
				send_event(event);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::on_stop(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_id.clear();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::on_uninitialize(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Event manager uninitializing...");

			if(nomic::core::thread::active()) {
				flush_events();
				nomic::core::thread::stop();
			}

			TRACE_MESSAGE(LEVEL_INFORMATION, "Event manager uninitialized");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::poll_event(
			__inout nomic::core::event &event
			)
		{
			bool result = false;

			TRACE_ENTRY(LEVEL_VERBOSE);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_queue.empty()) {
				event = m_queue.front();
				m_queue.pop();
				result = true;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		void 
		manager::receive_event(
			__in const nomic::core::event &event
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!nomic::core::thread::active()) {
				THROW_NOMIC_EVENT_MANAGER_EXCEPTION(NOMIC_EVENT_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			while(m_queue.size() >= QUEUE_MAX) {
				m_queue.pop();
			}

			m_queue.push(event);
			nomic::core::thread::notify();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::register_id(
			__in uint32_t id,
			__in nomic::event::queue *handle
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x, Handle=%p", id, handle);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!nomic::core::thread::active()) {
				THROW_NOMIC_EVENT_MANAGER_EXCEPTION(NOMIC_EVENT_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!handle) {
				THROW_NOMIC_EVENT_MANAGER_EXCEPTION_FORMAT(NOMIC_EVENT_MANAGER_EXCEPTION_HANDLE_INVALID, "Handle=%p", handle);
			}

			std::map<uint32_t, std::set<nomic::event::queue *>>::iterator iter = m_id.find(id);
			if(iter != m_id.end()) {

				if(iter->second.find(handle) != iter->second.end()) {
					THROW_NOMIC_EVENT_MANAGER_EXCEPTION_FORMAT(NOMIC_EVENT_MANAGER_EXCEPTION_DUPLICATE, "Handle=%p", handle);
				}

				iter->second.insert(handle);
			} else {
				std::set<nomic::event::queue *> entry;
				entry.insert(handle);
				m_id.insert(std::make_pair(id, entry));
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::registered_id(
			__in uint32_t id,
			__in_opt nomic::event::queue *handle
			)
		{
			bool result = false;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x, Handle=%p", id, handle);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!nomic::core::thread::active()) {
				THROW_NOMIC_EVENT_MANAGER_EXCEPTION(NOMIC_EVENT_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!handle) {
				THROW_NOMIC_EVENT_MANAGER_EXCEPTION_FORMAT(NOMIC_EVENT_MANAGER_EXCEPTION_HANDLE_INVALID, "Handle=%p", handle);
			}

			std::map<uint32_t, std::set<nomic::event::queue *>>::iterator iter = m_id.find(id);

			result = (iter != m_id.end());
			if(result && (handle != nullptr)) {
				result = (iter->second.find(handle) != iter->second.end());
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::send_event(
			__in nomic::core::event &event
			)
		{
			std::map<uint32_t, std::set<nomic::event::queue *>>::iterator iter;

			TRACE_ENTRY(LEVEL_VERBOSE);

			std::lock_guard<std::mutex> lock(m_mutex);

			iter = m_id.find(event.type());
			if(iter != m_id.end()) {

				for(std::set<nomic::event::queue *>::iterator entry = iter->second.begin(); entry != iter->second.end(); ++entry) {

					if(*entry != nullptr) {
						(*entry)->receive_event(event);
					}
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		manager::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_EVENT_MANAGER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Id[" << m_id.size() << "]";

				if(!m_id.empty()) {
					result << "={";

					for(std::map<uint32_t, std::set<nomic::event::queue *>>::const_iterator iter = m_id.begin(); iter != m_id.end();
							++iter) {

						if(iter != m_id.begin()) {
							result << ", ";
						}

						result << "(" << iter->first;

						if(!iter->second.empty()) {
							result << ", {";

							for(std::set<nomic::event::queue *>::const_iterator entry = iter->second.begin();
									entry != iter->second.end(); ++entry) {

								if(entry != iter->second.begin()) {
									result << ", ";
								}

								result << SCALAR_AS_HEX(uintptr_t, *entry);
							}

							result << "}";
						}

						result << ")";
					}

					result << "}";
				}

				result << ", Queue[" << m_queue.size() << "]";
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		manager::unregister_all_ids(
			__in_opt nomic::event::queue *handle
			)
		{
			std::set<uint32_t> cleanup;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p", handle);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!nomic::core::thread::active()) {
				THROW_NOMIC_EVENT_MANAGER_EXCEPTION(NOMIC_EVENT_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!handle) {
				THROW_NOMIC_EVENT_MANAGER_EXCEPTION_FORMAT(NOMIC_EVENT_MANAGER_EXCEPTION_HANDLE_INVALID, "Handle=%p", handle);
			}

			for(std::map<uint32_t, std::set<nomic::event::queue *>>::iterator iter = m_id.begin(); iter != m_id.end(); ++iter) {
				std::set<nomic::event::queue *>::iterator entry;

				for(entry = iter->second.begin(); entry != iter->second.end(); ++entry) {

					if(*entry == handle) {
						break;
					}
				}

				if(entry != iter->second.end()) {
					iter->second.erase(entry);
				}

				if(iter->second.empty()) {
					cleanup.insert(iter->first);
				}
			}

			for(std::set<uint32_t>::iterator iter = cleanup.begin(); iter != cleanup.end(); ++iter) {
				m_id.erase(find_id(*iter));
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::unregister_id(
			__in uint32_t id,
			__in_opt nomic::event::queue *handle
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x, Handle=%p", id, handle);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!nomic::core::thread::active()) {
				THROW_NOMIC_EVENT_MANAGER_EXCEPTION(NOMIC_EVENT_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!handle) {
				THROW_NOMIC_EVENT_MANAGER_EXCEPTION_FORMAT(NOMIC_EVENT_MANAGER_EXCEPTION_HANDLE_INVALID, "Handle=%p", handle);
			}

			std::map<uint32_t, std::set<nomic::event::queue *>>::iterator iter = m_id.find(id);
			if(handle != nullptr) {

				std::set<nomic::event::queue *>::iterator entry = iter->second.find(handle);
				if(entry == iter->second.end()) {
					THROW_NOMIC_EVENT_MANAGER_EXCEPTION_FORMAT(NOMIC_EVENT_MANAGER_EXCEPTION_HANDLE_NOT_FOUND, "Handle=%p", handle);
				}

				iter->second.erase(entry);

				if(iter->second.empty()) {
					m_id.erase(iter);
				}
			} else {
				m_id.erase(iter);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
