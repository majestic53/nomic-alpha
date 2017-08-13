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
#include "../../include/entity/chunk.h"
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
			__in nomic::core::renderer *renderer
			)
		{
			uint32_t type;
			std::map<uint32_t, std::pair<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p", renderer);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!renderer) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_INVALID,
					"Renderer=%p", renderer);
			}

			type = renderer->type();
			if(type > RENDERER_MAX) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_INVALID_TYPE,
					"Renderer=%p, Type=%x(%s)", renderer, type, RENDERER_STRING(type));
			}

			iter = m_entry.find(type);
			if(iter == m_entry.end()) {
				m_entry.insert(std::make_pair(type, std::pair<nomic::core::renderer *, std::set<nomic::core::entity *>>()));

				iter = m_entry.find(type);
				if(iter == m_entry.end()) {
					THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_NOT_FOUND,
						"Renderer=%p, Type=%x(%s)", renderer, type, RENDERER_STRING(type));
				}
			}

			iter->second.first = renderer;

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Renderer added. Renderer=%p, Type=%x(%s)", renderer, type, RENDERER_STRING(type));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		manager::contains(
			__in uint32_t type
			)
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x(%s)", type, RENDERER_STRING(type));

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(type > RENDERER_MAX) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_INVALID_TYPE,
					"Type=%x(%s)", type, RENDERER_STRING(type));
			}

			result = (m_entry.find(type) != m_entry.end());

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		bool 
		manager::contains_entity(
			__in nomic::core::entity *entity,
			__in uint32_t type
			)
		{
			bool result;
			std::set<nomic::core::entity *>::iterator iter_entity;
			std::map<uint32_t, std::pair<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Entity=%p, Type=%x(%s)", entity, type, RENDERER_STRING(type));

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!entity) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ENTITY_INVALID,
					"Entity=%p", entity);
			}

			if(type > RENDERER_MAX) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_INVALID_TYPE,
					"Entity=%p, Type=%x(%s)", entity, type, RENDERER_STRING(type));
			}

			iter = m_entry.find(type);

			result = (iter != m_entry.end());
			if(result) {
				iter_entity = iter->second.second.find(entity);
				result = (iter_entity != iter->second.second.end());
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		std::map<uint32_t, std::pair<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator 
		manager::find(
			__in uint32_t type
			)
		{
			std::map<uint32_t, std::pair<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x(%s)", type, RENDERER_STRING(type));

			if(type > RENDERER_MAX) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_INVALID_TYPE,
					"Type=%x(%s)", type, RENDERER_STRING(type));
			}

			result = m_entry.find(type);
			if(result == m_entry.end()) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_NOT_FOUND,
					"Type=%x(%s)", type, RENDERER_STRING(type));
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

			m_entry.clear();

			TRACE_MESSAGE(LEVEL_INFORMATION, "Render manager uninitialized.");

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::remove(
			__in nomic::core::renderer *renderer
			)
		{
			uint32_t type;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p", renderer);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!renderer) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_INVALID,
					"Renderer=%p", renderer);
			}

			type = renderer->type();
			if(type > RENDERER_MAX) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_INVALID_TYPE,
					"Renderer=%p, Type=%x(%s)", renderer, type, RENDERER_STRING(type));
			}

			m_entry.erase(find(type));

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Renderer removed. Renderer=%p, Type=%x(%s)", renderer, type,
				RENDERER_STRING(type));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::register_entity(
			__in nomic::core::entity *entity,
			__in uint32_t type
			)
		{
			std::map<uint32_t, std::pair<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Entity=%p, Type=%x(%s)", entity, type, RENDERER_STRING(type));

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!entity) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ENTITY_INVALID,
					"Entity=%p", entity);
			}

			if(type > RENDERER_MAX) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_INVALID_TYPE,
					"Entity=%p, Type=%x(%s)", entity, type, RENDERER_STRING(type));
			}

			iter = find(type);
			if(iter->second.second.find(entity) != iter->second.second.end()) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ENTITY_DUPLICATE,
					"Entity=%p, Type=%x(%s)", entity, type, RENDERER_STRING(type));
			}

			iter->second.second.insert(entity);

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Entity registered. Entity=%p, Renderer=%p, Type=%x(%s)",
				entity, iter->second.first, type, RENDERER_STRING(type));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::render(
			__in const glm::vec3 &position,
			__in const glm::vec3 &rotation,
			__in const glm::mat4 &projection,
			__in const glm::mat4 &view,
			__in const glm::uvec2 &view_dimensions,
			__in nomic::graphic::atlas &textures,
			__in float cycle,
			__in float delta,
			__in const glm::vec4 &ambient,
			__in const glm::vec4 &ambient_background,
			__in const glm::vec3 &ambient_position,
			__in bool clouds,
			__in bool underwater
			)
		{
			std::map<uint32_t, std::pair<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE,
"Position=%p, Rotation=%p, Projection=%p, View=%p, Dimensions={%u, %u}, Textures=%p, Cycle=%f, Delta=%f, Ambient={%f, %f, %f, %f} (Background={%f, %f, %f, %f}, Position={%f, %f, %f}), Clouds=%x, Underwater=%x",
				&position, &rotation, &projection, &view, view_dimensions.x, view_dimensions.y, &textures, cycle, delta,
				ambient.x, ambient.y, ambient.z, ambient.w, ambient_background.x, ambient_background.y, ambient_background.z,
				ambient_background.w, ambient_position.x, ambient_position.y, ambient_position.z, clouds, underwater);

			std::lock_guard<std::mutex> lock(m_mutex);

			for(iter = m_entry.begin(); iter != m_entry.end(); ++iter) {
				nomic::core::renderer *rend = iter->second.first;
				std::set<nomic::core::entity *> &entity = iter->second.second;

				if(!rend || entity.empty()) {
					continue;
				}

				switch(rend->mode()) {
					case RENDER_PERSPECTIVE:
						rend->use(position, rotation, cycle, ambient, ambient_background, ambient_position, clouds, underwater,
							projection, view);
						break;
					default:
						rend->use(position, rotation, cycle, ambient, ambient_background, ambient_position, clouds, underwater,
							glm::ortho(0.f, (float) view_dimensions.x, 0.f, (float) view_dimensions.y, -1.f, 1.f));
						break;
				}

				for(std::set<nomic::core::entity *>::iterator iter_entity = entity.begin(); iter_entity != entity.end();
						++iter_entity) {

					if(!*iter_entity || !(*iter_entity)->shown()) {
						continue;
					}

					rend->set_model((*iter_entity)->model());
					(*iter_entity)->on_render(*rend, &textures, delta);
				}

				if(iter->first == RENDERER_CHUNK) {

					for(std::set<nomic::core::entity *>::iterator iter_entity = entity.begin(); iter_entity != entity.end();
							++iter_entity) {

						if(!*iter_entity || !(*iter_entity)->shown()) {
							continue;
						}

						rend->set_model((*iter_entity)->model());
						((nomic::entity::chunk *) (*iter_entity))->on_render_transparent(*rend, &textures, delta);
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
			std::set<nomic::core::entity *>::const_iterator iter_entity;
			std::map<uint32_t, std::pair<nomic::core::renderer *, std::set<nomic::core::entity *>>>::const_iterator iter;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_RENDER_MANAGER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << SINGLETON_CLASS(nomic::render::manager)::to_string(verbose)
					<< ", Entry[" << m_entry.size() << "]";

				if(!m_entry.empty()) {
					result << "={";

					for(iter = m_entry.begin(); iter != m_entry.end(); ++iter) {

						if(iter != m_entry.begin()) {
							result << ", ";
						}

						result << SCALAR_AS_HEX(uintptr_t, iter->second.first) << "[" << iter->second.second.size() << "]";

						if(!iter->second.second.empty()) {
							result << "={";

							for(iter_entity = iter->second.second.begin(); iter_entity != iter->second.second.end();
									++iter_entity) {

								if(iter_entity != iter->second.second.begin()) {
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

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		manager::unregister_all_entities(
			__in uint32_t type
			)
		{
			std::map<uint32_t, std::pair<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x(%s)", type, RENDERER_STRING(type));

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(type > RENDERER_MAX) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_INVALID_TYPE,
					"Type=%x(%s)", type, RENDERER_STRING(type));
			}

			iter = find(type);
			iter->second.second.clear();

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "All entities unregistered. Renderer=%p, Type=%x(%s)",
				iter->second.first, type, RENDERER_STRING(type));

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		manager::unregister_entity(
			__in nomic::core::entity *entity,
			__in uint32_t type
			)
		{
			std::set<nomic::core::entity *>::iterator iter_entity;
			std::map<uint32_t, std::pair<nomic::core::renderer *, std::set<nomic::core::entity *>>>::iterator iter;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Entity=%p, Type=%x(%s)", entity, type, RENDERER_STRING(type));

			std::lock_guard<std::mutex> lock(m_mutex);

			if(!m_initialized) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION(NOMIC_RENDER_MANAGER_EXCEPTION_UNINITIALIZED);
			}

			if(!entity) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ENTITY_INVALID,
					"Entity=%p", entity);
			}

			if(type > RENDERER_MAX) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_INVALID_TYPE,
					"Entity=%p, Type=%x(%s)", entity, type, RENDERER_STRING(type));
			}

			iter = m_entry.find(type);
			if(iter == m_entry.end()) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_RENDERER_NOT_FOUND,
					"Entity=%p, Type=%x(%s)", entity, type, RENDERER_STRING(type));
			}

			iter_entity = iter->second.second.find(entity);
			if(iter_entity == iter->second.second.end()) {
				THROW_NOMIC_RENDER_MANAGER_EXCEPTION_FORMAT(NOMIC_RENDER_MANAGER_EXCEPTION_ENTITY_NOT_FOUND,
					"Entity=%p, Type=%x(%s)", entity, type, RENDERER_STRING(type));
			}

			iter->second.second.erase(iter_entity);

			TRACE_MESSAGE_FORMAT(LEVEL_INFORMATION, "Entity unregistered. Entity=%p, Renderer=%p, Type=%x(%s)",
				entity, iter->second.first, type, RENDERER_STRING(type));

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
