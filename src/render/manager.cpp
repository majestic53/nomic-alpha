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
			GLuint id;
			std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter_id;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p", handle);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!handle) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_HANDLE_INVALID, "Handle=%p", handle);
			}

			id = handle->get_id();

			iter_id = m_id.find(id);
			if(iter_id != m_id.end()) {

				if(iter_id->second.find(handle) != iter_id->second.end()) {
					THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_HANDLE_DUPLICATE,
						"Handle=%p", handle);
				}
			} else {
				m_id.insert(std::make_pair(id, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>()));

				iter_id = m_id.find(id);
				if(iter_id == m_id.end()) {
					THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_HANDLE_NOT_FOUND,
						"Handle=%p", handle);
				}
			}

			iter_id->second.insert(std::make_pair(handle, std::set<nomic::core::entity *>()));

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Renderer added. Handle=%p", handle);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::contains(
			__in GLuint id
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			std::lock_guard<std::mutex> lock(m_mutex);

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

			std::lock_guard<std::mutex> lock(m_mutex);

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
			std::set<nomic::core::entity *>::iterator iter_entity;
			std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter_id;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p, Id=%x", handle, id);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!handle) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ENTITY_HANDLE_INVALID, "Handle=%p", handle);
			}

			iter_id = find_id(id);

			for(std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>::iterator iter_handle = iter_id->second.begin();
					iter_handle != iter_id->second.end(); ++iter_handle) {

				iter_entity = iter_handle->second.find(handle);
				if(iter_entity != iter_handle->second.end()) {
					THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ENTITY_HANDLE_DUPLICATE,
						"Handle=%p", handle);
				}

				iter_handle->second.insert(handle);
			}

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Entity registered. Handle=%p", handle);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::remove(
			__in nomic::core::renderer *handle
			)
		{
			std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>::iterator iter_handle;
			std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter_id;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p", handle);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			iter_handle = find_handle(handle);
			iter_id = find_id(handle->get_id());
			iter_id->second.erase(iter_handle);

			if(iter_id->second.empty()) {
				m_id.erase(iter_id);
			}

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Renderer removed. Handle=%p", handle);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::render(
			__in const glm::vec3 &position,
			__in const glm::mat4 &projection,
			__in const glm::mat4 &view,
			__in const glm::uvec2 &view_dimensions,
			__in float delta
			)
		{
			std::map<nomic::core::renderer *, std::set<nomic::core::entity *>> deferred;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Projection=%p, View=%p, Delta=%f", &projection, &view, delta);

			std::lock_guard<std::mutex> lock(m_mutex);

			for(std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter_id = m_id.begin();
					iter_id != m_id.end(); ++iter_id) {

				for(std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>::iterator iter_handle = iter_id->second.begin();
						iter_handle != iter_id->second.end(); ++iter_handle) {

					if(!iter_handle->first || iter_handle->second.empty()) {
						continue;
					}

					switch(iter_handle->first->mode()) {
						case RENDER_PERSPECTIVE:
							iter_handle->first->use(position, projection, view);
							break;
						default:
							iter_handle->first->use(position, glm::ortho(0.f, (float) view_dimensions.x, 0.f,
								(float) view_dimensions.y, -1.f, 1.f));
							break;
					}

					for(std::set<nomic::core::entity *>::iterator iter_entity = iter_handle->second.begin();
							iter_entity != iter_handle->second.end(); ++iter_entity) {

						if(!*iter_entity || !(*iter_entity)->shown()) {
							continue;
						} else if((*iter_entity)->deferred()) {
							std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>::iterator iter_deferred;

							iter_deferred = deferred.find(iter_handle->first);
							if(iter_deferred == deferred.end()) {
								deferred.insert(std::make_pair(iter_handle->first, std::set<nomic::core::entity *>()));
								iter_deferred = deferred.find(iter_handle->first);
							}

							iter_deferred->second.insert(*iter_entity);
						} else {
							iter_handle->first->set_model((*iter_entity)->model());
							(*iter_entity)->on_render(*iter_handle->first, delta);
						}
					}
				}
			}

			for(std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>::iterator iter_handle = deferred.begin();
					iter_handle != deferred.end(); ++iter_handle) {

				if(!iter_handle->first || iter_handle->second.empty()) {
					continue;
				}

				switch(iter_handle->first->mode()) {
					case RENDER_PERSPECTIVE:
						iter_handle->first->use(position, projection, view);
						break;
					default:
						iter_handle->first->use(position, glm::ortho(0.f, (float) view_dimensions.x, 0.f,
							(float) view_dimensions.y, -1.f, 1.f));
						break;
				}

				for(std::set<nomic::core::entity *>::iterator iter_entity = iter_handle->second.begin();
						iter_entity != iter_handle->second.end(); ++iter_entity) {

					if(!*iter_entity || !(*iter_entity)->shown()) {
						continue;
					}

					iter_handle->first->set_model((*iter_entity)->model());
					(*iter_entity)->on_render(*iter_handle->first, delta);
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

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			m_id.erase(find_id(id));
			TRACE_MESSAGE(LEVEL_INFORMATION, "Unregistered all entities.");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::unregister_entity(
			__in nomic::core::entity *handle,
			__in GLuint id
			)
		{
			std::set<nomic::core::entity *>::iterator iter_entity;
			std::map<GLuint, std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter_id;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Handle=%p, Id=%x", handle, id);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!handle) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ENTITY_HANDLE_INVALID, "Handle=%p", handle);
			}

			iter_id = find_id(id);

			for(std::map<nomic::core::renderer *, std::set<nomic::core::entity *>>::iterator iter_handle = iter_id->second.begin();
					iter_handle != iter_id->second.end(); ++iter_handle) {

				iter_entity = iter_handle->second.find(handle);
				if(iter_entity != iter_handle->second.end()) {
					iter_handle->second.erase(iter_entity);
				}
			}

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Unregisted entity. Handle=%p", handle);

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
