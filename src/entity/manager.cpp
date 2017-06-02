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

#include "../../include/entity/manager.h"
#include "../../include/trace.h"
#include "./manager_type.h"

namespace nomic {

	namespace entity {

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

		void 
		manager::add(
			__in nomic::core::entity *handle
			)
		{
			uint32_t id;
			std::map<uint32_t, std::set<nomic::core::entity *>>::iterator iter;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p", handle);

			if(!m_initialized) {
				THROW_NOMIC_ENTITY_MANAGER_EXCEPTION(NOMIC_ENTITY_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!handle) {
				THROW_NOMIC_ENTITY_MANAGER_EXCEPTION_FORMAT(NOMIC_ENTITY_MANAGER_EXCEPTION_HANDLE_INVALID, "Handle=%p", handle);
			}

			id = handle->get_id();
			if(id == UID_INVALID) {
				THROW_NOMIC_ENTITY_MANAGER_EXCEPTION_FORMAT(NOMIC_ENTITY_MANAGER_EXCEPTION_ID_INVALID, "Id=%x", id);
			}

			iter = m_id.find(id);
			if(iter != m_id.end()) {

				if(iter->second.find(handle) != iter->second.end()) {
					THROW_NOMIC_ENTITY_MANAGER_EXCEPTION_FORMAT(NOMIC_ENTITY_MANAGER_EXCEPTION_DUPLICATE_HANDLE,
						"Handle=%p", handle);
				}
			} else {
				m_id.insert(std::make_pair(id, std::set<nomic::core::entity *>()));
				iter = find_id(id);
			}

			iter->second.insert(handle);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::contains(
			__in uint32_t id
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			if(!m_initialized) {
				THROW_NOMIC_ENTITY_MANAGER_EXCEPTION(NOMIC_ENTITY_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(id == UID_INVALID) {
				THROW_NOMIC_ENTITY_MANAGER_EXCEPTION_FORMAT(NOMIC_ENTITY_MANAGER_EXCEPTION_ID_INVALID, "Id=%x", id);
			}

			result = (m_id.find(id) != m_id.end());

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		std::set<nomic::core::entity *>::iterator 
		manager::find_handle(
			__in nomic::core::entity *handle,
			__in std::map<uint32_t, std::set<nomic::core::entity *>>::iterator iter
			)
		{
			std::set<nomic::core::entity *>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p", handle);

			if(!handle) {
				THROW_NOMIC_ENTITY_MANAGER_EXCEPTION_FORMAT(NOMIC_ENTITY_MANAGER_EXCEPTION_HANDLE_INVALID, "Handle=%p", handle);
			}

			result = iter->second.find(handle);
			if(result == iter->second.end()) {
				THROW_NOMIC_ENTITY_MANAGER_EXCEPTION_FORMAT(NOMIC_ENTITY_MANAGER_EXCEPTION_HANDLE_NOT_FOUND, "Handle=%p", handle);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		std::map<uint32_t, std::set<nomic::core::entity *>>::iterator 
		manager::find_id(
			__in uint32_t id
			)
		{
			std::map<uint32_t, std::set<nomic::core::entity *>>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			if(id == UID_INVALID) {
				THROW_NOMIC_ENTITY_MANAGER_EXCEPTION_FORMAT(NOMIC_ENTITY_MANAGER_EXCEPTION_ID_INVALID, "Id=%x", id);
			}

			result = m_id.find(id);
			if(result == m_id.end()) {
				THROW_NOMIC_ENTITY_MANAGER_EXCEPTION_FORMAT(NOMIC_ENTITY_MANAGER_EXCEPTION_ID_NOT_FOUND, "Id=%x", id);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		bool 
		manager::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Entity manager initializing...");
			TRACE_MESSAGE(LEVEL_INFORMATION, "Entity manager initialized");

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::on_uninitialize(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Entity manager uninitializing...");

			m_id.clear();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Entity manager uninitialized");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::remove(
			__in nomic::core::entity *handle
			)
		{
			std::set<nomic::core::entity *>::iterator iter_handle;
			std::map<uint32_t, std::set<nomic::core::entity *>>::iterator iter_id;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p", handle);

			if(!m_initialized) {
				THROW_NOMIC_ENTITY_MANAGER_EXCEPTION(NOMIC_ENTITY_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			iter_id = find_id(handle->get_id());
			iter_handle = find_handle(handle, iter_id);
			iter_id->second.erase(iter_handle);

			if(iter_id->second.empty()) {
				m_id.erase(iter_id);
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

			result << NOMIC_ENTITY_MANAGER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Id[" << m_id.size() << "]";

				if(!m_id.empty()) {
					result << "={";

					for(std::map<uint32_t, std::set<nomic::core::entity *>>::const_iterator iter_id = m_id.begin();
							iter_id != m_id.end(); ++iter_id) {

						if(iter_id != m_id.begin()) {
							result << ", ";
						}

						result << "(" << SCALAR_AS_HEX(uint32_t, iter_id->first) << "[" << iter_id->second.size() << "]";

						if(!iter_id->second.empty()) {
							result << "={";

							for(std::set<nomic::core::entity *>::const_iterator iter_handle = iter_id->second.begin();
									iter_handle != iter_id->second.end(); ++iter_handle) {

								if(iter_handle != iter_id->second.begin()) {
									result << ", ";
								}

								result << SCALAR_AS_HEX(uintptr_t, *iter_handle);
							}

							result << "}";
						}

						result << ")";
					}

					result << "}";
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		manager::update(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_initialized) {
				THROW_NOMIC_ENTITY_MANAGER_EXCEPTION(NOMIC_ENTITY_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			for(std::map<uint32_t, std::set<nomic::core::entity *>>::iterator iter_id = m_id.begin(); iter_id != m_id.end();
					++iter_id) {

				for(std::set<nomic::core::entity *>::iterator iter_handle = iter_id->second.begin();
						iter_handle != iter_id->second.end(); ++iter_handle) {

					if(!*iter_handle || !(*iter_handle)->enabled()) {
						continue;
					}

					(*iter_handle)->update();
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
