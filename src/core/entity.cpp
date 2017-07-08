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

#include "../../include/core/entity.h"
#include "../../include/entity/manager.h"
#include "../../include/render/manager.h"
#include "../../include/trace.h"
#include "./entity_type.h"

namespace nomic {

	namespace core {

		entity::entity(
			__in uint32_t type,
			__in uint32_t subtype,
			__in_opt const glm::vec3 &position,
			__in_opt const glm::vec3 &rotation,
			__in_opt const glm::vec3 &up
			) :
				nomic::core::object(type, subtype),
				nomic::core::transform(position, rotation, up),
				m_deferred(false),
				m_enabled(true),
				m_shown(true)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Subtype=%x", type, subtype);

			add();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		entity::entity(
			__in const entity &other
			) :
				nomic::core::id(other),
				nomic::core::object(other),
				nomic::core::transform(other),
				m_deferred(other.m_deferred),
				m_enabled(other.m_enabled),
				m_shown(other.m_shown)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			add();
			register_renderers(other.m_renderer);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		entity::~entity(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			unregsiter_all_renderers();
			remove();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		entity &
		entity::operator=(
			__in const entity &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				unregsiter_all_renderers();
				remove();
				nomic::core::id::operator=(other);
				nomic::core::object::operator=(other);
				nomic::core::transform::operator=(other);
				m_deferred = other.m_deferred;
				m_enabled = other.m_enabled;
				m_shown = other.m_shown;
				add();
				register_renderers(other.m_renderer);
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", this);
			return *this;
		}

		void 
		entity::add(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::entity::manager &instance = nomic::entity::manager::acquire();
			if(instance.initialized()) {
				instance.add(this);
			}

			instance.release();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		entity::defer(
			__in bool state
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "State=%x", state);

			m_deferred = state;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		entity::deferred(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_deferred);
			return m_deferred;
		}

		void 
		entity::enable(
			__in bool state
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "State=%x", state);

			m_enabled = state;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		entity::enabled(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_enabled);
			return m_enabled;
		}

		void 
		entity::on_render(
			__in nomic::core::renderer &renderer,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Delta=%f", &renderer, delta);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		entity::on_update(
			__in void *runtime,
			__in void *camera
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Runtime=%p, Camera=%p", runtime, camera);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		entity::registered(
			__in GLuint id
			) const
		{
			bool result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			result = (m_renderer.find(id) != m_renderer.end());

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", result);
			return result;
		}

		void 
		entity::register_renderer(
			__in GLuint id
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			if(id == HANDLE_INVALID) {
				THROW_NOMIC_CORE_ENTITY_EXCEPTION_FORMAT(NOMIC_CORE_ENTITY_EXCEPTION_RENDERER_INVALID, "Id=%x", id);
			}

			if(m_renderer.find(id) != m_renderer.end()) {
				THROW_NOMIC_CORE_ENTITY_EXCEPTION_FORMAT(NOMIC_CORE_ENTITY_EXCEPTION_RENDERER_DUPLICATE, "Id=%x", id);
			}

			nomic::render::manager &instance = nomic::render::manager::acquire();
			if(instance.initialized()) {
				instance.register_entity(this, id);
			}

			instance.release();
			m_renderer.insert(id);

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		entity::register_renderers(
			__in const std::set<GLuint> &renderer
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer[%u]=%p", renderer.size(), &renderer);

			for(std::set<GLuint>::const_iterator iter = renderer.begin(); iter != renderer.end(); ++iter) {
				register_renderer(*iter);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		entity::remove(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::entity::manager &instance = nomic::entity::manager::acquire();
			if(instance.initialized() && instance.contains(m_id)) {
				instance.remove(this);
			}

			instance.release();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		entity::set_view_dimensions(
			__in const glm::vec2 &view_dimensions
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "View={%u, %u}", view_dimensions.x, view_dimensions.y);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		entity::show(
			__in bool state
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "State=%x", state);

			m_shown = state;

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		bool 
		entity::shown(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%x", m_shown);
			return m_shown;
		}

		std::string 
		entity::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_CORE_ENTITY_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Object=" << nomic::core::object::to_string(verbose)
					<< ", Transform=" << nomic::core::transform::to_string(verbose)
					<< ", Id=" << nomic::core::id::to_string(verbose)
					<< ", State=" << (m_enabled ? "Enabled" : "Disabled") << "/" << (m_shown ? "Shown" : "Hidden")
						<< "/" << (m_deferred ? "Deferred" : "Immediate")
					<< ", Renderer[" << m_renderer.size() << "]";

				if(!m_renderer.empty()) {
					result << "={";

					for(std::set<GLuint>::iterator iter = m_renderer.begin(); iter != m_renderer.end(); ++iter) {

						if(iter != m_renderer.begin()) {
							result << ", ";
						}

						result << SCALAR_AS_HEX(GLuint, *iter);
					}

					result << "}";
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		void 
		entity::unregsiter_all_renderers(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			nomic::render::manager &instance = nomic::render::manager::acquire();

			for(std::set<GLuint>::iterator iter = m_renderer.begin(); iter != m_renderer.end(); ++iter) {

				if(instance.initialized() && instance.contains_registration(this, *iter)) {
					instance.unregister_entity(this, *iter);
				}
			}

			instance.release();
			m_renderer.clear();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		entity::unregister_renderer(
			__in GLuint id
			)
		{
			std::set<GLuint>::iterator result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Id=%x", id);

			if(id == HANDLE_INVALID) {
				THROW_NOMIC_CORE_ENTITY_EXCEPTION_FORMAT(NOMIC_CORE_ENTITY_EXCEPTION_RENDERER_INVALID, "Id=%x", id);
			}

			result = m_renderer.find(id);
			if(result == m_renderer.end()) {
				THROW_NOMIC_CORE_ENTITY_EXCEPTION_FORMAT(NOMIC_CORE_ENTITY_EXCEPTION_RENDERER_UNREGISTERED, "Id=%x", id);
			}

			nomic::render::manager &instance = nomic::render::manager::acquire();
			if(instance.initialized() && instance.contains_registration(this, id)) {
				instance.unregister_entity(this, id);
			}

			instance.release();
			m_renderer.erase(result);

			TRACE_EXIT(LEVEL_VERBOSE);
		}
	}
}
