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

#include "../../include/uuid/manager.h"
#include "../../include/trace.h"
#include "./manager_type.h"

namespace nomic {

	namespace uuid {

		manager::manager(void) :
			m_id_next(UID_INVALID + 1)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		manager::~manager(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::clear(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			m_id.clear();
			m_id_next = (UID_INVALID + 1);
			m_id_surplus.clear();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::contains(
			__in uint32_t id
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_UUID_MANAGER_EXCEPTION(NOMIC_UUID_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(id == UID_INVALID) {
				THROW_NOMIC_UUID_MANAGER_EXCEPTION_FORMAT(NOMIC_UUID_MANAGER_EXCEPTION_INVALID, "Id=%x", id);
			}

			result = (m_id.find(id) != m_id.end());

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		size_t 
		manager::decrement(
			__in uint32_t id
			)
		{
			size_t result = 0;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_UUID_MANAGER_EXCEPTION(NOMIC_UUID_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			std::map<uint32_t, size_t>::iterator iter = find(id);
			if(iter->second <= REFERENCE_INIT) {

				if(m_id_surplus.find(iter->first) != m_id_surplus.end()) {
					THROW_NOMIC_UUID_MANAGER_EXCEPTION_FORMAT(NOMIC_UUID_MANAGER_EXCEPTION_DUPLICATE, "Id=%x", iter->first);
				}

				m_id_surplus.insert(iter->first);
				m_id.erase(iter);
			} else {
				result = --iter->second;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		std::map<uint32_t, size_t>::iterator 
		manager::find(
			__in uint32_t id
			)
		{
			std::map<uint32_t, size_t>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			if(id == UID_INVALID) {
				THROW_NOMIC_UUID_MANAGER_EXCEPTION_FORMAT(NOMIC_UUID_MANAGER_EXCEPTION_INVALID, "Id=%x", id);
			}

			result = m_id.find(id);
			if(result == m_id.end()) {
				THROW_NOMIC_UUID_MANAGER_EXCEPTION_FORMAT(NOMIC_UUID_MANAGER_EXCEPTION_NOT_FOUND, "Id=%x", id);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		uint32_t 
		manager::generate(void)
		{
			uint32_t result = UID_INVALID;

			TRACE_ENTRY(LEVEL_VERBOSE);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_UUID_MANAGER_EXCEPTION(NOMIC_UUID_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!m_id_surplus.empty()) {
				std::set<uint32_t>::iterator iter = m_id_surplus.begin();
				result = *iter;
				m_id_surplus.erase(iter);
			} else if(m_id_next != UID_INVALID) {
				result = m_id_next++;
			} else {
				THROW_NOMIC_UUID_MANAGER_EXCEPTION(NOMIC_UUID_MANAGER_EXCEPTION_FULL);
			}

			if(result == UID_INVALID) {
				THROW_NOMIC_UUID_MANAGER_EXCEPTION_FORMAT(NOMIC_UUID_MANAGER_EXCEPTION_INVALID, "Id=%x", result);
			}

			if(m_id.find(result) != m_id.end()) {
				THROW_NOMIC_UUID_MANAGER_EXCEPTION_FORMAT(NOMIC_UUID_MANAGER_EXCEPTION_DUPLICATE, "Id=%x", result);
			}

			m_id.insert(std::make_pair(result, REFERENCE_INIT));

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		size_t 
		manager::increment(
			__in uint32_t id
			)
		{
			size_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_UUID_MANAGER_EXCEPTION(NOMIC_UUID_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			result = ++find(id)->second;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		bool 
		manager::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Uuid manager initializing...");
			TRACE_MESSAGE(LEVEL_INFORMATION, "Uuid manager initialized");

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::on_uninitialize(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Uuid manager uninitializing...");

			clear();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Uuid manager uninitialized");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		size_t 
		manager::references(
			__in uint32_t id
			)
		{
			size_t result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_UUID_MANAGER_EXCEPTION(NOMIC_UUID_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			result = find(id)->second;

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%u", result);
			return result;
		}

		std::string 
		manager::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_UUID_MANAGER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << SINGLETON_CLASS(nomic::uuid::manager)::to_string(verbose);

				if(m_initialized) {
					result << ", Id[" << m_id.size() << "]";

					if(!m_id.empty()) {
						result << "={";

						for(std::map<uint32_t, size_t>::const_iterator iter = m_id.begin(); iter != m_id.end(); ++iter) {

							if(iter != m_id.begin()) {
								result << ", ";
							}

							result << "(" << SCALAR_AS_HEX(uint32_t, iter->first) << ", Reference=" << iter->second << ")";
						}

						result << "}";
					}

					result << ", Surplus[" << m_id_surplus.size() << "]";

					if(!m_id_surplus.empty()) {
						result << "={";

						for(std::set<uint32_t>::const_iterator iter = m_id_surplus.begin(); iter != m_id_surplus.end();
								++iter) {

							if(iter != m_id_surplus.begin()) {
								result << ", ";
							}

							result << SCALAR_AS_HEX(uint32_t, *iter);
						}

						result << "}";
					}

					result << ", Next=" << SCALAR_AS_HEX(uint32_t, m_id_next);
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}
	}
}
