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

#include "../../include/entity/object.h"
#include "../../include/trace.h"
#include "./object_type.h"

namespace nomic {

	namespace entity {

		object::object(
			__in uint32_t type,
			__in uint32_t subtype,
			__in_opt const glm::vec3 &position,
			__in_opt const glm::vec3 &rotation,
			__in_opt const glm::vec3 &up,
			__in_opt const glm::mat4 &model,
			__in_opt const glm::mat4 &projection,
			__in_opt const glm::mat4 &view
			) :
				nomic::core::entity(type, subtype, position, rotation, up),
				nomic::core::uniform(model, projection, view),
				m_vao(nullptr),
				m_view_dimensions(DISPLAY_DEFAULT_WIDTH, DISPLAY_DEFAULT_HEIGHT)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Type=%x, Subtype=%x", type, subtype);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		object::object(
			__in const object &other
			) :
				nomic::core::entity(other),
				nomic::core::uniform(other),
				m_vao(other.m_vao),
				m_view_dimensions(other.m_view_dimensions)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		object::~object(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(m_vao) {
				delete m_vao;
				m_vao = nullptr;
			}

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		object &
		object::operator=(
			__in const object &other
			)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(this != &other) {
				nomic::core::entity::operator=(other);
				nomic::core::uniform::operator=(other);
				m_vao = other.m_vao;
				m_view_dimensions = other.m_view_dimensions;
			}

			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Result=%p", this);
			return *this;
		}

		glm::mat4 &
		object::model(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
			return nomic::core::uniform::model(m_position, m_rotation);
		}

		void 
		object::on_render(
			__in nomic::core::renderer &renderer,
			__in float delta
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Renderer=%p, Delta=%f", &renderer, delta);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		object::on_update(
			__in void *runtime,
			__in void *camera
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Runtime=%p, Camera=%p", runtime, camera);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		object::on_view_change(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT(LEVEL_VERBOSE);
		}

		void 
		object::set_view_dimensions(
			__in const glm::vec2 &view_dimensions
			)
		{
			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "View={%u, %u}", view_dimensions.x, view_dimensions.y);

			m_view_dimensions = view_dimensions;
			on_view_change();

			TRACE_EXIT(LEVEL_VERBOSE);
		}

		std::string 
		object::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT(LEVEL_VERBOSE, "Verbose=%x", verbose);

			result << NOMIC_ENTITY_OBJECT_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Entity=" << nomic::core::entity::to_string(verbose)
					<< ", Uniform=" << nomic::core::uniform::to_string(verbose);
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return result.str();
		}

		nomic::graphic::vao &
		object::vertex_array(void)
		{
			TRACE_ENTRY(LEVEL_VERBOSE);

			if(!m_vao) {

				m_vao = new nomic::graphic::vao;
				if(!m_vao) {
					THROW_NOMIC_ENTITY_OBJECT_EXCEPTION(NOMIC_ENTITY_OBJECT_EXCEPTION_ALLOCATE);
				}
			}

			TRACE_EXIT(LEVEL_VERBOSE);
			return *m_vao;
		}

		glm::uvec2 
		object::view_dimensions(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "View={%u, %u}", m_view_dimensions.x, m_view_dimensions.y);
			return m_view_dimensions;
		}

		uint32_t 
		object::view_height(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Height=%u", m_view_dimensions.y);
			return m_view_dimensions.y;
		}

		uint32_t 
		object::view_width(void) const
		{
			TRACE_ENTRY(LEVEL_VERBOSE);
			TRACE_EXIT_FORMAT(LEVEL_VERBOSE, "Width=%u", m_view_dimensions.x);
			return m_view_dimensions.x;
		}
	}
}
