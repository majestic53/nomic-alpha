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

#include "../../include/render/manager.h"
#include "../../include/trace.h"
#include "./manager_type.h"

namespace nomic {

	namespace render {

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
			__in nomic::core::renderer *handle
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p", handle);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			// TODO

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::contains(
			__in GLuint id
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(id == HANDLE_INVALID) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ID_INVALID, "Id=%x", id);
			}

			result = (m_id.find(id) != m_id.end());

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		manager::contains_registration(
			__in nomic::core::entity *handle,
			__in GLuint id
			)
		{
			bool result = false;
			std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter_id;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p, Id=%x", handle, id);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(id == HANDLE_INVALID) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ID_INVALID, "Id=%x", id);
			}

			if(!handle) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ENTITY_HANDLE_INVALID, "Handle=%p", handle);
			}

			iter_id = m_id.find(id);

			result = (iter_id != m_id.end());
			if(result) {

				for(std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>::iterator iter_handle = iter_id->second.begin();
						iter_handle != iter_id->second.end(); ++iter_id) {

					result = (iter_handle->second.find(handle) != iter_handle->second.end());
					if(result) {
						break;
					}
				}
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>::iterator 
		manager::find_handle(
			__in nomic::core::renderer *handle
			)
		{
			std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>::iterator result;
			std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator handles;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p", handle);

			if(!handle) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_HANDLE_INVALID, "Handle=%p", handle);
			}

			handles = find_id(handle->get_id());

			result = handles->second.find(handle);
			if(result == handles->second.end()) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_HANDLE_NOT_FOUND, "Handle=%p", handle);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator 
		manager::find_id(
			__in GLuint id
			)
		{
			std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			if(id == HANDLE_INVALID) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ID_INVALID, "Id=%x", id);
			}

			result = m_id.find(id);
			if(result == m_id.end()) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ID_NOT_FOUND, "Id=%x", id);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result;
		}

		bool 
		manager::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Render manager initializing...");
			TRACE_MESSAGE(LEVEL_INFORMATION, "Render manager initialized.");

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		manager::on_uninitialize(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			TRACE_MESSAGE(LEVEL_INFORMATION, "Render manager uninitializing...");

			m_id.clear();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Render manager uninitialized.");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::register_entity(
			__in nomic::core::entity *handle,
			__in GLuint id
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p, Id=%x", handle, id);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			// TODO

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::remove(
			__in nomic::core::renderer *handle
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p", handle);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			// TODO

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::render(
			__in glm::mat4 &projection,
			__in glm::mat4 &view,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Projection=%p, View=%p, Delta=%f", &projection, &view, delta);

			for(std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter_id = m_id.begin();
					iter_id != m_id.end(); ++iter_id) {

				for(std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>::iterator iter_handle = iter_id->second.begin();
						iter_handle != iter_id->second.end(); ++iter_handle) {

					if(!iter_handle->first || iter_handle->second.empty()) {
						continue;
					}

					iter_handle->first->use(projection, view);

					for(std::set<nomic::core::entity *>::iterator iter_entity = iter_handle->second.begin();
							iter_entity != iter_handle->second.end(); ++iter_entity) {

						if(!*iter_entity || !(*iter_entity)->shown()) {
							continue;
						}

						(*iter_entity)->render(delta);
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

			result << NOMIC_RENDER_MANAGER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Id[" << m_id.size() << "]";

				if(!m_id.empty()) {
					result << "={";

					for(std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>>::const_iterator iter_id
							= m_id.begin(); iter_id != m_id.end(); ++iter_id) {

						if(iter_id != m_id.begin()) {
							result << ", ";
						}

						result << SCALAR_AS_HEX(GLuint, iter_id->first) << "[" << iter_id->second.size() << "]";

						if(!iter_id->second.empty()) {
							result << "={";

							for(std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>::const_iterator
									iter_handle = iter_id->second.begin(); iter_handle != iter_id->second.end();
									++iter_handle) {

								if(iter_handle != iter_id->second.begin()) {
									result << ", ";
								}

								result << SCALAR_AS_HEX(uintptr_t, iter_handle->first)
									<< "[" << iter_handle->second.size() << "]";

								if(!iter_handle->second.empty()) {
									result << "={";

									for(std::set<nomic::core::entity *>::const_iterator iter_entity
											= iter_handle->second.begin();
											iter_entity != iter_handle->second.end(); ++iter_entity) {

										if(iter_entity != iter_handle->second.begin()) {
											result << ", ";
										}

										result << SCALAR_AS_HEX(uintptr_t, *iter_entity);
									}

									result << "}";
								}
							}

							result << "}";
						}
					}

					result << "}";
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		manager::unregister_all_entities(
			__in GLuint id
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			// TODO

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::unregister_entity(
			__in nomic::core::entity *handle,
			__in GLuint id
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p, Id=%x", handle, id);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			// TODO

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
